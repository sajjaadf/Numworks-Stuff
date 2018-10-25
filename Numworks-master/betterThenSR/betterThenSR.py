import random
a1 = [
"Familiar as it is,",
"You should commit to",
"There never should be",
"There always should be,",
"Why there is the idea of",
"The objection to",
"But why should you",
"A foolish consistency",
"When you converse with people,",
]
a2 = [
"keeping your head",
"a person who likes to walk,",
"in whatever variety of actions",
]
a3 = [
"is of the utmost importance.",
"we do not know.",
"as is all things.",
"is not a good one.",
]

r1 = [
"These varieties are lost sight of at a little distance, at a little height of thought.",
"Your genuine action will explain itself, and will explain your other genuine actions. Your conformity explains nothing.",
"All the foregone days of virtue work their health into this. What makes the majesty of the heroes of the senate and the field, which so fills the imagination?",
"I hope in these days we have heard the last of conformity and consistency.",
"I will stand here for humanity, and though I would make it kind, I would make it true.",
]

ramblings = [
"If malice and vanity wear the coat of philanthropy, shall that pass?",
"We must return to nature and be with each other.",
"How so? It is quite obvious once you see the diacritical hypothesis of quantum entanglement.",
"Then, again, do not tell me, as a good man did to-day, of my obligation to put all poor men in good situations. ",
"I cannot consent to pay for a privilege where I have intrinsic right.",
"We come to wear one cut of face and figure, and acquire by degrees the gentlest asinine expression.",
]

paragraph = []
c = " "
def paragraphF():
    paragraph.append(random.choice(a1))
    paragraph.append(random.choice(a2))
    paragraph.append(random.choice(a3))
    paragraph.append(random.choice(r1))
    paragraph.append(random.choice(ramblings))
    paragraph.append(random.choice(ramblings))
    result = c.join(paragraph)
    print("=====================")
    lim=75
    for s in result.split("\n"):
        if s == "": print
        w=0
        l = []
        for d in s.split():
            if w + len(d) + 1 <= lim:
                l.append(d)
                w += len(d) + 1
            else:
                print(" ".join(l))
                l = [d]
                w = len(d)
        if (len(l)): print(" ".join(l))
    print("=====================")
    del paragraph[:]
print("=============")
print("How many paragraphs?")
n = int(input(":"))
for i in range(n) :
    paragraphF()
n = input()
