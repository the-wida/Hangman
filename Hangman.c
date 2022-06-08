#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)

void interface();
void body_print(int wrong_answer_amount);
void main()
{
	int i, j, word_length, answer_amount, correct_one, temp_same, correct_word, correct_answer_amount;
	int wrong_answer_amount, current_answer_right, actual_word_amount, letter_appeared_true, got_same_last_true;
	int option, random_number, amount_of_words, scores[10], pos, switchtemp, none;
	char *word, *answers, *right_answers, *wrong_answers, tempstring[255], *names[10];
	char temp, tempstringscores[255], player_name[225], temp_for_number[255], wipeopt;
	FILE *words_file;
	FILE *scores_file;
	while (1)
	{
		system("cls");
		interface();
		scanf_s("%d", &option);
		switch (option)
		{
		case 1:
			word = malloc(0);
			//vvv Gets the amount of possible words
			words_file = fopen("words.txt", "r");
			if (words_file == NULL)
			{
				system("cls");
				printf("Error: txt file missing\n\n");
				system("pause");
				return;
			}
			rewind(words_file);
			amount_of_words = 0;
			while (1)
			{
				fgets(temp_for_number, 255, words_file);
				if (feof(words_file))
				{
					break;
				}
				else amount_of_words++;
			}
			rewind(words_file);
			//vvv Gets a random sentence from the txt file
			srand(time(NULL));
			random_number = rand() % (amount_of_words+1);
			for (i = 0; i <=random_number; i++)
			{
				fgets(tempstring, 255, (FILE*)words_file);
			}
			word_length = strlen(tempstring);
			word = realloc(word, word_length * sizeof(char));
			strcpy_s(word, (word_length+1) * sizeof(char), tempstring);
			fclose(words_file);
			answer_amount = 0;
			answers = malloc(0);
			right_answers = malloc(0);
			wrong_answers = malloc(0);
			correct_word = 0;
			correct_answer_amount = 0;
			wrong_answer_amount = 0;
			actual_word_amount = 0;
			got_same_last_true = 0;
			none = 0;
			//vvv Determines the amount of unique characters in the word
			for (i = 0; i < word_length-1; i++)
			{
				if (word[i] != ' ')
				{
					if (i == 0)
					{
						actual_word_amount = 1;
					}
					else
					{
						actual_word_amount++;
						letter_appeared_true = 0;
						for (j = i - 1; j >= 0; j--)
						{
							if (word[j] == word[i])
							{
								letter_appeared_true = 1;
							}
						}
						if (letter_appeared_true == 1)
							actual_word_amount--;
					}
				}
			}
			while (correct_word == 0)
			{
				system("cls");
				if (got_same_last_true == 1)
					printf("Sorry, you already guessed that letter\n");
				got_same_last_true = 0;
				//vvv Prints out the guessed and not yet guessed parts of the word
				for (i = 0; i < word_length-1; i++)
				{
					if (word[i] == ' ')
						printf("  ");
					else {
						correct_one = 0;
						for (j = 0; j < answer_amount; j++)
						{
							if (answers[j]< '[' && answers[j] >'@')
							{
								if (answers[j] == word[i] || answers[j]+32 == word [i])
									correct_one = 1;
							}
							else if (answers[j]<'{' && answers[j] > 96)
							{
								if (answers[j] == word[i] || answers[j] - 32 == word[i])
									correct_one = 1;
							}
						}
						if (correct_one == 1)
							printf("%c ", word[i]);
						else printf("_ ");
					}
				}
				printf("\n");
				//vvv Prints out all the previous guesses, or 'none' if there were no previous guesses
				printf("\nPrevious answers:");
				if (none == 0)
				{
					printf(" none.");
					none = 1;
				}
				else
				{
					for (i = 0; i < answer_amount; i++)
					{
						printf("%c", answers[i]);
						if (i != answer_amount - 1)
							printf(", ");
						else printf(".");
					}
				}
				body_print(wrong_answer_amount);
				//vvv Algorythm for taking the user's next guess
				printf("\nEnter your next guess ::>");
				temp = '\n';
				while (temp == '\n')
				{
					scanf_s("%c", &temp);
					if (temp != '\n')
					{
						temp_same = 0;
						for (i = 0; i < answer_amount; i++)
						{
							if (temp<'{'&& temp > 96)
							{
								if (temp == answers[i] || temp - 32 == answers[i])
								{
									temp_same = 1;
								}
							}
							else if (temp < '[' && temp > '@')
							{
								if (temp == answers[i] || temp + 32 == answers[i])
								{
									temp_same = 1;
								}
							}
						}
						if (temp_same == 0)
						{
							current_answer_right = 0;
							answer_amount++;
							answers = realloc(answers, answer_amount * sizeof(char));
							answers[answer_amount - 1] = temp;
							for (j = 0; j < word_length-1; j++)
							{
								if (temp<'{'&& temp > 96)
								{
									if (word[j] == temp || word[j] == temp - 32)
									{
										current_answer_right = 1;
									}
								}
								else if (temp < '[' && temp > '@')
								{
									if (word[j] == temp || word[j] == temp + 32)
									{
										current_answer_right = 1;
									}
								}
							}
							if (current_answer_right == 1)
							{
								correct_answer_amount++;
								right_answers = realloc(right_answers, correct_answer_amount * sizeof(char));
								right_answers[correct_answer_amount - 1] = temp;
							}
							else
							{
								wrong_answer_amount++;
								wrong_answers = realloc(wrong_answers, wrong_answer_amount * sizeof(char));
								wrong_answers[wrong_answer_amount - 1] = temp;
							}
						}
						else got_same_last_true = 1;
					}
				}
				//vvv Checks to see if the user got the entire word right
				if (correct_answer_amount == actual_word_amount)
				{
					correct_word = 1;
					system("cls");
					for (i = 0; i < word_length-1; i++)
					{
						printf("%c ", word[i]);
					}
					printf("\n\nCongrats, you got it right\n");
				//vvv This part will deal with high score table entries
					scores_file = fopen("scores.txt", "r+");
					if (scores_file == NULL)
					{
						system("cls");
						printf("Error: txt file missing\n\n");
						system("pause");
						return;
					}
					rewind(scores_file);
					for (i = 0; i < 10; i++)
					{
						fgets(tempstringscores, 5 * sizeof(char), (FILE*)scores_file);
						if (i != 0)
							fgets(tempstringscores, 5 * sizeof(char), (FILE*)scores_file);
						names[i] = malloc((strlen(tempstringscores) + 1) * sizeof(char));
						strcpy_s(names[i], (strlen(tempstringscores) + 1) * sizeof(char), tempstringscores);
					}
					for (i = 0; i < 10; i++)
					{
						fscanf((FILE*)scores_file, "%d", &scores[i]);
					}
					pos = 10;
					for (i =9; i >= 0; i--)
					{
						if (wrong_answer_amount < scores[i])
							pos--;
					}
					if (pos < 10)
					{
						printf("\nEligible for the high score table entry!\n\n");
						printf("Please enter a 4 character name::>");
						gets(&player_name);
						gets(&player_name);
					}
					player_name[4] = '\0';
					if (pos < 10)
					{
						for (i = 8; i >= pos; i--)
						{
							scores[i + 1] = scores[i];
							strcpy_s(names[i+1], (strlen(names[i]) + 1) * sizeof(char), names[i]);
						}
						strcpy_s(names[pos], (5) * sizeof(char), player_name);
						scores[pos] = wrong_answer_amount;
						rewind(scores_file);
						for (i = 0; i < 10; i++)
						{
							fprintf(scores_file,"%s\n", names[i]);
						}
						for (i = 0; i < 10; i++)
						{
							fprintf(scores_file, "%d\n", scores[i]);
						}
					}
					fclose(scores_file);
				}
				//vvv Checks to see if the user ran out of tries
				if (wrong_answer_amount == 6)
				{
					correct_word = 1;
					system("cls");
					body_print(6);
					printf("Sorry, you were hanged\n\n");
				}
			}
			system("pause");
			break;
	    case 2 :
			system("cls");
			printf("------------------------------------------------------------\n");
			printf("                    High score table:\n");
			printf("------------------------------------------------------------\n");
			printf("#      NAME   MISTAKES\n");
			printf("------------------------------------------------------------\n");
			scores_file = fopen("scores.txt", "r");
			if (scores_file == NULL)
			{
				system("cls");
				printf("Error: txt file missing\n\n");
				system("pause");
				return;
			}
			rewind(scores_file);
			for (i = 0; i <10; i++)
			{ 
				fgets(tempstringscores, 5 * sizeof(char), (FILE*)scores_file);
				if (i!=0)
			    fgets(tempstringscores, 5 * sizeof(char), (FILE*)scores_file);
				names[i] = malloc ((strlen(tempstringscores)+1) * sizeof(char));
				strcpy_s(names[i], (strlen(tempstringscores)+1) * sizeof(char), tempstringscores);
			}
			for (i = 0; i < 10; i++)
			{
				fscanf((FILE*)scores_file, "%d", &scores[i]);
			}
			for (i = 0; i < 10; i++)
			{
				printf("%d.----", (i + 1));
				if (i != 9)
				printf("-");
				printf("%s------", names[i]);
				printf("%d", scores[i]);
				printf("\n");
			}
			printf("------------------------------------------------------------\n");
			printf("\n\n");
			fclose(scores_file);
			system("pause");
			break;
		case 3:
			system("cls");
			printf("WARNING\n\n");
			printf("This will reset the contents of the high score table\n");
			printf("Proceed? (y/n)\n");
			scanf("%c", &wipeopt);
			scanf("%c", &wipeopt);
			switch (wipeopt)
			{
			case 'y':
				scores_file = fopen("scores.txt", "w");
				if (scores_file == NULL)
				{
					system("cls");
					printf("Error: txt file missing\n\n");
					system("pause");
					return;
				}
				rewind(scores_file);
				for (i = 0; i < 10; i++)
				{
					fputs("AAAA\n", scores_file);
				}
				for (i = 0; i < 10; i++)
				{
					fprintf(scores_file, "%d\n", 100);
				}
				fclose(scores_file);
				system("cls");
				printf("Succesfully wiped\n\n");
				system("pause");
				break;
			case 'n':
				system("cls");
				printf("Wiping canceled\n\n");
				system("pause");
				break;
			default:
				system("cls");
				printf("Unknown option, options are y and n\nTry again\n\n");
				system("pause");
				break;
			}
			break;
		case 4:
			system("cls");
			printf("Hangman is a game in which you are given a random word and have to guess\n");
			printf("it letter by letter, if you get a letter right, it will appear visible at\n");
			printf("the top, if you get it wrong, you get another body part added to your hangman.\n");
			printf("If your hangman is completed, you are hanged and lose the game.\n");
			printf("In the case that you win, you will potentially be able to enter your name into\n");
			printf("the high score table.\n");
			printf("To add more words modify the words txt file, to manually change some scores\n");
			printf("modify the scores txt file, you can also reset the table using the option #3.\n\n\n");
			system("pause");
			break;
		case 5: 
			system("cls");
			printf("HANGMAN\n");
			printf("Made by Slaven Vidakovic, 2018\n");
			printf("Written in c using Visual Studio 2017\n\n\n");
			system("pause");
			break;
		case 0:
			exit(1);
		default:
			printf("Unknown command, try again\n");
			system("pause");
		}
	}
}
//vvv Responsible for the main interface
void interface()
{
	printf("------------------------------------------------------------\n");
	printf("Welcome to Hangman\n");
	printf("Please choose an option\n");
	printf("1. Play a game\n");
	printf("2. Display the high score table\n");
	printf("3. Clear the high score table\n");
	printf("4. Rules\n");
	printf("5. About\n");
	printf("0. Exit\n");
	printf("::>");
}
//vvv Responsible for drawing the stickman
void body_print(int wrong_answer_amount)
{
	printf("\n\n* * * * * * *\n");
	printf("*  _____    *\n");
	printf("*  |   |    *\n");
	printf("*  |   ");
	if (wrong_answer_amount > 0)
	printf("O");
	else
	printf(" ");
	printf("    *\n");
	printf("*  |  ");
	if (wrong_answer_amount > 2)
	printf("/");
	else
	printf(" ");
	if (wrong_answer_amount > 1)
	printf("I");
	else
	printf(" ");
	if (wrong_answer_amount > 3)
	printf("\\");
	else
	printf(" ");
	printf("   *\n");
	printf("*  |  ");
	if (wrong_answer_amount > 4)
	printf("/ ");
	else
	printf("  ");
	if (wrong_answer_amount > 5)
	printf("\\");
	else
	printf(" ");
	printf("   *\n");
	printf("* _I_____   * \n");
	printf("* * * * * * *\n");
}