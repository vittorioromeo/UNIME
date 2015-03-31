vowels = 'aeiouy'
starts = ['ou','ei','ae','ea','eu','oi']
endings = ['es','ed','e']

def syllables(word):
    acc = 0
    word = word.lower().strip(".:;?!")
    
    for vowel in vowels:
        acc +=word.count(vowel)
    
    for ending in endings:
        if word.endswith(ending):
            acc -=1
    
    	for start in starts:
        if word.startswith(start):
            acc -=1
    
    if word.endswith('le'):
        acc +=1
    
    if acc == 0:
        acc+=1
    
    return acc

def getSentenceCount(text):
	return text.count(".")

def getSyllableCount(text):
	acc = 0
	for word in text.split():
		acc += syllables(word)
	
	return acc

def getWordCount(text):
	return len(text.split())

def getFleshIndex(text):	
	print "Words: " + str(getWordCount(text))
	print "Syllables: " + str(getSyllableCount(text))
	print "Sentences: " + str(getSentenceCount(text))

	wordSentenceRatio = (getWordCount(text) / getSentenceCount(text))
	syllableWordRatio = (getSyllableCount(text) / getWordCount(text))
	return 206.835 - 1.015 * wordSentenceRatio - 84.6 * syllableWordRatio


with open('example.tx') as f: text = f.read()

print getFleshIndex(text)