from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
import datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route('/add-cash', methods=["GET", "POST"])
@login_required
def add_cash():
    """ add cash to your account """
    
    if request.method == "POST":
        # check if amount was provided
        if not request.form.get("amount"):
            return apology("Please enter a cash amount to add to your balance")
        
        # add the cash amount to the current account
        db.execute("update users set cash = cash + :amount where id=:user_id",
                    amount=request.form.get("amount"),
                    user_id=session['user_id'])
        return redirect(url_for("index"))
        
    else:
        return render_template("add-cash.html")

@app.route("/")
@login_required
def index():
    """ Load the main page """
    
    # get user data for current login user
    user = db.execute("select * from users where id=:user_id",
                       user_id=session["user_id"])[0]
    if not user:
        return apology("error loading user data")
    
    # get the user's current stock amount by summing the transactions per stock
    portfolio = db.execute("select stock_symbol, sum(amount) as total_amount \
                            from transactions \
                            where user_id = :user_id group by stock_symbol",
                            user_id=user["id"])

    
    # lookup each stock to get the current value of the portfolio
    total = 0
    for stock in portfolio:
        s = lookup(stock["stock_symbol"])
        stock["name"] = s["name"]
        stock["price"] = s["price"]
        total += stock["price"] * stock["total_amount"]

    return render_template("index.html", user=user,
                            portfolio=portfolio, total=total)
    

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    if request.method == "POST":
        # check if a symbol was submitted
        if not request.form.get("symbol"):
            return apology("you must provide a valid stock symbol")
            
        # check if a positive amount was submitted
        if (not request.form.get("amount")
                or not int(request.form.get("amount")) > 0):
            return apology("you can only buy 1 or more stocks",
                           "please provide a larger amount")
            
        # lookup the current stock price for the submitted symbol
        quote = lookup(request.form.get("symbol"))
        # check if a quote was found for the symbol
        if not quote:
            return apology("could not find the stock symbol",
                           "please try another")
        
        # get the current user's cash balance and 
        # check if it is sufficient to buy the requested stock
        user_balance = db.execute("select cash from users where id=:user_id",
                                   user_id=session["user_id"])[0]
        total_value = int(request.form.get("amount")) * quote["price"]
        if not user_balance["cash"] > total_value:
            return apology("you don't have enough money!")
        
        # store the transaction in the transactions table
        tx = db.execute("insert into transactions (user_id, stock_symbol, amount, price, date) \
                        values (:user_id, :stock_symbol, :amount, :price, datetime('now'))",
                        user_id=session["user_id"], stock_symbol=quote["symbol"],
                        amount=int(request.form.get("amount")), price=quote["price"])

        if not tx:
            return apology("ERROR try again")
            
        # update the user's cash balance after
        db.execute("update users set cash = cash - :txvalue where id=:user_id",
                    txvalue=total_value, user_id=session["user_id"])

        # redirect to the index page
        return redirect(url_for('index'))
        
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    # load the users transactions by date
    txs = db.execute("select * from transactions where user_id = :user order by date asc",
                      user=session["user_id"])
    return render_template("history.html",transactions=txs)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                           username=request.form.get("username"))

        # ensure username exists and password is correct
        if (len(rows) != 1
                or not pwd_context.verify(request.form.get("password"), rows[0]["hash"])):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET
    # (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    if request.method == "POST":
        # check if a symbol was submitted
        if not request.form.get("symbol"):
            return apology("you must provide a stock symbol")
        
        # get the quote and check if it is valid
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("could not find the stock symbol", "please try another")
            
        # show the quote
        return render_template("quoted.html", quote=quote)
        
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    if request.method == "POST":
        # check is a username was entered
        if not request.form.get("username"):
            return apology("must provide a username")
            
        # check if passwords match
        if not request.form.get("password") == request.form.get("password-confirm"):
            return apology("passwords don't match")
        
        # store the new user's data in the users table
        user_id = db.execute("insert into users (username, hash) values (:username, :password)",
                            username=request.form.get("username"),
                            password=pwd_context.hash(request.form.get("password")))
        
        # if the user_id is None than the username already exists
        if user_id == None:
            return apology("username already exists")
        
        # store the user_id in the session   
        session["user_id"] = user_id
            
        # redirect to the index page
        return redirect(url_for("index"))
            
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    if request.method == "POST":
        # check if a stock symbol was provided
        if not request.form.get("symbol"):
            return apology("Please provide the stock you want to sell")
            
        # check if an amount was provided
        if not request.form.get("amount"):
            return apology("Please provide the amount you want to sell")
        
        # check if the user actually owns enough of the stock to sell it
        in_stock = db.execute("select sum(amount) as total_amount from transactions \
                               where user_id = :user_id and stock_symbol = :symbol",
                               user_id=session["user_id"],
                               symbol=request.form.get("symbol"))[0]["total_amount"]
        if int(request.form.get("amount")) > in_stock:
            return apology("You are trying to sell more than you own!", "Don't do that!")
            
        # lookup the current price
        price = lookup(request.form.get("symbol"))["price"]
        
        # calculate the value and add a sell transaction
        value = price * int(request.form.get("amount"))
        tx = db.execute("insert into transactions (user_id, stock_symbol, amount, price, date) \
                         values (:user_id, :stock_symbol, :amount, :price, datetime('now'))",
                        user_id=session["user_id"], stock_symbol=request.form.get("symbol"),
                        amount=-int(request.form.get("amount")), price=price)
        if not tx:
            return apology("ERROR try again")
        
        # update the user's cash balance    
        db.execute("update users set cash = cash + :txvalue where id=:user_id",
                    txvalue=value, user_id=session["user_id"])
        
        return redirect(url_for("index"))
        
    else:
        portfolio = db.execute("select stock_symbol, sum(amount) as total_amount \
                                from transactions where user_id = :user_id \
                                group by stock_symbol",
                                user_id=session["user_id"])
        return render_template("sell.html", portfolio=portfolio)