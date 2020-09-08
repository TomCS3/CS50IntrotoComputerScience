from cs50 import get_string


# Prompts user for text to assess
text = get_string("Text: ")


def main():

    # Calls functions to assign values
    nlets = count_letters()
    nwords = count_words()
    nsents = count_sents()

    # Coleman-Liau index formula
    index = round(100 * (0.0588 * nlets / nwords - 0.296 * nsents / nwords) - 15.8)

    # Prints grade based on index
    if index < 1:
        print("Before Grade 1")
    elif index < 16:
        print(f"Grade {index}")
    else:
        print("Grade 16+")


# Function to count the number of letters
def count_letters():
    nlets = 0
    for char in text:
        if(char.isalpha()):
            nlets += 1
    return nlets


# Function to count the number of words
def count_words():
    nwords = len(text.split())
    return nwords


# Function to count the number of sentences
def count_sents():
    nsents = 0
    for char in text:
        if(char == '.' or char == '!' or char == '?'):
            nsents += 1
    return nsents


main()