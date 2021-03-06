
#include "utils.hpp"
class App
{
public:
  App()
  {
    // this value is gonna be important if you create a question in time execution
    statements["choose"] = "Elije la respuesta correcta";

    // this are all the paths for every tale
    path[1] = "questionsdb/tale1";
    path[2] = "questionsdb/tale2";
    path[3] = "questionsdb/tale3";
  };

  void setQuestion(int key, string statement, string question, vector<string> options, int answer, string idiom)
  {
    // you have to provide path
    ofstream setMyFile(path[1] + "/questions.en.txt", ios::app);

    // attempt to open the questions.txt
    if (setMyFile.is_open())
    {
      // writing the statement
      setMyFile << key << ". " << statements[statement] << "\n";
      // writing the question
      setMyFile << question << "\n";
      for (int i = 0; i < options.size(); i++)
      {
        // writing the options, and separate them with the commas
        setMyFile << options[i] << ",";
      }
      // writing the answer
      setMyFile << "\n"
                << "answ)" << answer;
    }
    setMyFile.close();
  }

  void start(string idiom, int tale)
  {

    idiom == "en" ? myfile.open(path[tale] + "/questions.en.txt") : myfile.open(path[tale] + "/questions.fr.txt");

    if (myfile.is_open())
    {
      getNumberOfApp(myfile, totalQuestions);

      return getQuestion(FIRST_QUESTION);
    }
    else
    {
      cout << "error on opening the file";
      return;
    }
  }

  void getQuestion(int questionWanted)
  {

    string line = "";
    regex optionsEx{","};
    regex answerEx{"answ."};
    array<string, TOTAL_OPTIONS_SIZE> options;
    int i = 0;
    currentQuestion = questionWanted;

    // first you need to go to your specific question number
    // " -1 " because the first question starts at line 0, son the second one starts on line 4... and so on...
    int lineNumber = (questionWanted - 1) * LINES_PER_QUESTION;
    if (lineNumber != 0)
      goToQuestion(myfile, lineNumber);

    while (getline(myfile, line) && i < 5)
    {
      i++;

      if (regex_search(line, optionsEx))
      {

        options = split(line, ',');

        continue;
      }
      // looking for the answer
      if (regex_search(line, answerEx))
      {
        size_t pos = line.find(")");
        answer = stoi(line.substr(pos + 1));
        continue;
      }

      // when i is 5, you will print the statement of the next question
      if (i != 5)
      {
        cout << line << endl;
      }
    }

    for (int i = 0; i < options.size(); i++)
    {

      if (trim(options[i]) == "")
        continue;

      cout << "\t" << i + 1 << ") " << options[i] << "\t";
    }
    cout << endl;

    return evaluate();
  }

  void savingQuestion()
  {
    for (int i = 0; i < questionsCorrect.size(); i++)
    {

      if (questionsCorrect[i] == 0)
      {
        switch (currentQuestion)
        {
        case 1:
          questionsCorrect[i] = 35;
          break;
        case 2:
          questionsCorrect[i] = 40;
          break;
        case 3:
          questionsCorrect[i] = 72;
          break;
        case 4:
          questionsCorrect[i] = 14;
          break;
        case 5:
          questionsCorrect[i] = 9;
          break;
        default:
          break;
        }

        return;
      }
    }
  }

  void evaluate()
  {
    int toEvaluate = -1;

    cout << "choose your answer: ";
    cin >> toEvaluate;
    int nextQuestion = currentQuestion + 1;
    if (toEvaluate == answer)
    {
      correct++;
      cout << "great job!\n";
      // saving the question on the array
      savingQuestion();
      // if all App all completed, program ends
    }
    else
    {
      cout << "You commit an error:(\n";
      cout << "The correct answer was: " << answer << "\n";
    }
    // this is just to separate questions between them
    cout << "\n\n";

    if (nextQuestion > totalQuestions)
    {
      // if all questions all completed, program ends

      myfile.close();
      return userScore();
    }

    return getQuestion(nextQuestion);
  }
  void insertionSort()
  {

    int i, key, j;

    for (i = 1; i < questionsCorrect.size(); i++)
    {
      key = questionsCorrect[i];
      j = i - 1;
      while (j >= 0 && questionsCorrect[j] > key)
      {
        questionsCorrect[j + 1] = questionsCorrect[j];
        j = j - 1;
      }
      questionsCorrect[j + 1] = key;
    }

    return;
  }

  void userScore()
  {
    auto sumCorrectAnswers = [&]()
    {
      int sum = 0;
      for (int i = 0; i < questionsCorrect.size(); i++)
      {
        sum += questionsCorrect[i];
      }
      return sum;
    }();

    auto printCorrectAnswers = [&]()
    {
      for (int i = 0; i < questionsCorrect.size(); i++)
      {
        if (questionsCorrect[i] != 0)
          cout << i << ") " << questionsCorrect[i] << ", ";
      }

      return;
    };

    insertionSort();
    cout << "The user score is:\n";
    cout << "Total number of correct answers: " << getNumberCorrectAnswers() << endl;
    cout << "Percentage of correct answers: " << fixed << setprecision(2) << getPercentageCorrectAnswers() << "%" << endl;
    cout << "Score per answer :" << endl;
    printCorrectAnswers();

    cout << "\nPuntaje total: " << sumCorrectAnswers << endl;

    return;
  }

  int getNumberCorrectAnswers()
  {
    return correct;
  }

  float getPercentageCorrectAnswers()
  {
    return ((double)correct / (double)totalQuestions) * 100;
  }

  ~App(){};

private:
  map<string, string> statements;
  map<int, string> path;
  int answer = -1;
  ifstream myfile;
  array<int, 5> questionsCorrect = {0, 0, 0, 0, 0};

  int currentQuestion = -1;
  int totalQuestions = -1;
  int correct = 0;
};