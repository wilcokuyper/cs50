import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = []
        for line in open(positives):
            # skip lines starting with ; or a space
            if line[0] != ';' and line[0] != ' ':
                self.positives.append(line.strip())
                
        self.negatives = []
        for line in open(negatives):
            # skip lines starting with ; or a space
            if line[0] != ';' and line[0] != ' ':
                self.negatives.append(line.strip())

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        score = 0.0;

        words = text.split(' ')
        # match each word in either the positives or negatives list adding or subtracting 1 from the score if present
        for word in words:
            for w in self.positives:
                if w == word.lower():
                    score += 1.0
                    continue
                
            for w in self.negatives:
                if w == word.lower():
                    score -= 1.0
                    continue

        return score
