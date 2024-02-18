
def main():
    
    inpupt_text = str(input("Text: "))

    grade = get_readability(inpupt_text)

    


    


def get_readability(text: str) -> int:
    
    letter_counter = len([ele for ele in text if ele.isalpha()])
    print(letter_counter)

    word_count = len(text.split())
    print(word_count)

    sentence_couter = len(text.split(sep=". "))
    sentence_couter += len(text.split(sep="! ")) -1
    sentence_couter += len(text.split(sep="? ")) -1
    print(sentence_couter)

    L = letter_counter / word_count * 100
    S = sentence_couter / word_count * 100
    #Coleman-Liau index 
    #L average number of letters per 100 words
    # S average number of sentences per 100 words in the text
    readability =  round (0.0588 * L - 0.296 * S - 15.8)
    if readability < 1:
        readability = 1
        print(f"Before Grrade {readability}")
        return readability
    
    if readability >= 16:
        readability = 16

    print(f"Grade {readability}+")
    return readability

main()
