#include "raylib.h"
#include <vector>
#include <cmath>
#include <stdio.h>
#include <string>
#include <stdlib.h>

// Default screen Width, Height and min widnow size
#define ScreenWidth 1200
#define ScreenHeight 800

//Setting the minimum screen width and height
#define MinWindowWidth 500
#define MinWindowHeight 800



// Assigning Numerical Values to the states of pillars
#define NORMAL 0
#define COMPARING 1
#define SORTED 2
#define SELECTED 3
#define MINIMUM 4
#define PIVOT 5
#define LEFT 6
#define RIGHT 7

//Setting the FPS
#define FPS 120

using namespace std;
// Size of Array of numbers
int NumOfBars = 40;



// Defining the Functions
void ShowStartingScreen(); // First screen the user sees
void ShowOptions();// Screen shown on selecting a sorting technique
void ShowEndingScreen();

void Button(float x, float y, char *Text, Color color, bool &state);//Position of button, text in the button and the state

void Start_Button(float size, float font, char Start[]);
void Selection_Sort_Button(float size, char Selection_Sort_Text[]);
void Insertion_Sort_Button(float size, char Insertion_Sort_Text[]);
void Bubble_Sort_Button(float size, char Bubble_Sort_Text[]);
void Merge_Sort_Button(float size, char Merge_Sort_Text[]);


Color FindColorForPiller(int pillerState);
void DrawArray( vector< pair<int, int>> arr);
void RandomizeArray( vector< pair<int, int>> &arr);

// Sorting Functions Prototypes:
void SortArray();

void Selection_Sort( vector< pair<int, int>> &arr);

void Insertion_Sort( vector< pair<int, int>> &arr);

void Bubble_Sort( vector< pair<int, int>> &arr);


void Merge_Sort( vector< pair<int, int>> &arr);
void MergeSort_Finale( vector< pair<int, int>> &arr,
    int leftIndex, int rightIndex);
void Merge( vector< pair<int, int>> &arr,
    int leftIndex, int m, int rightIndex);


// Changing size and speed
void ChangeSize(char operation, int &value);
void ChangeSpeed(char operation, int &value);

// Game States:
bool ShouldShowStartingScreen = true;
bool ShouldShowOptions = false;
bool GameShouldStart = false;
bool ShouldShowEndingScreen = false;



bool addSpeed = false;
bool subSpeed = false;
bool addSize = false;
bool subSize = false;
bool NormalSize = false;
bool NormalSpeed = false;


bool SelectionSortPressed = false;
bool InsertionSortPressed = false;
bool BubbleSortPressed = false;
bool MergeSortPressed = false;
 vector<bool *> SortingChoice = {
    &SelectionSortPressed,
    &InsertionSortPressed,
    &BubbleSortPressed,
    &MergeSortPressed
};

bool ShouldRandomize = true;

char* convertIntegerToChar(int N) // Function to change the integer to char array to show the speed of sorting
{

    int m = N;
    int digit = 0;
    while (m) {
        digit++;
        m /= 10;
    }

    char* arr;

    char arr1[digit];

    
    arr = (char*)malloc(digit);
    
    int index = 0;
    while (N) {

        arr1[++index] = N % 10 + '0';
        N /= 10;
    }

    int i;
    for (i = 0; i < index; i++) {
        arr[i] = arr1[index - i];
    }

    arr[i] = '\0';

    return (char*)arr;
}

// Array of Values and It's States;
 vector< pair<int, int>> arr(NumOfBars);//globally declared the vector named arr
int SortingSpeed = 41;



int main(){

    // Window Configuration:
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(ScreenWidth, ScreenHeight, "Sorting Algorithms :)");
    SetWindowMinSize(MinWindowWidth, MinWindowHeight);

    // Set FPS
    SetTargetFPS(FPS);

    while (!WindowShouldClose())
        {
        BeginDrawing();
        ClearBackground(DARKPURPLE);
        
        if (ShouldRandomize)
        {
            RandomizeArray(arr);
        }
        if (ShouldShowStartingScreen){
            ShowStartingScreen();
        }
        ShouldShowOptions = false;
        for (bool *i : SortingChoice){
            if (*i == true)
                ShouldShowOptions = true;
        }

        if (ShouldShowOptions){
            ShowOptions();
        }


        if (GameShouldStart){
            ShouldShowStartingScreen = false;
            ShouldShowOptions = false;

            SetTargetFPS(SortingSpeed);
            SortArray();

            GameShouldStart = false;
            ShouldShowEndingScreen = true;
        }

        if (ShouldShowEndingScreen){
            ShowEndingScreen();
        }
        EndDrawing();
    }
    CloseWindow();
    return 1;
}
void ShowStartingScreen(){
    float font = (2.5 * GetScreenWidth()) / 100;
    char Selection_Sort_text[] = "Selection Sort";
    char Insertion_Sort_Text[] = "Insertion Sort";
    char Bubble_Sort_Text[] = "Bubble Sort";
    char Merge_Sort_Text[] = "Merge Sort";
    float tmp = (GetScreenWidth() * 15)/100;
    char Group[] = "Sorting Visualizer made by Shivansh, Nitin, Aryan and Sanskar";
    DrawText(Group, 60, ScreenHeight/20, font+5, YELLOW);
    Selection_Sort_Button(tmp, Selection_Sort_text);
    tmp += MeasureText(Selection_Sort_text, font) + font;
    Bubble_Sort_Button(tmp, Bubble_Sort_Text);
    tmp += MeasureText(Bubble_Sort_Text, font) + font;
    Insertion_Sort_Button(tmp, Insertion_Sort_Text);
    tmp += MeasureText(Insertion_Sort_Text, font) + font;
    Merge_Sort_Button(tmp, Merge_Sort_Text);
    tmp += MeasureText(Merge_Sort_Text, font) + font;
    DrawArray(arr);
}


void ShowOptions(){
    float font = (2.5 * GetScreenWidth()) / 100;
    char StartText[] = "Start Game!";
    float tmp = (25*GetScreenWidth()) / 100;
    Start_Button(tmp, font, StartText);
    tmp += MeasureText(StartText, font) + 75;
    char RandomizeArrayText[] = "Randomize Array!";
    Button(tmp, GetScreenHeight()/8 + font*2,
        RandomizeArrayText, RAYWHITE, ShouldRandomize);


    addSpeed = false;
    subSpeed = false;
    addSize = false;
    subSize = false;

    NormalSize = false;
    NormalSpeed = false;

    tmp = (80.2 * GetScreenWidth()) / 100;
    char TimeButtonText[] = "Speed ";
    strcat(TimeButtonText, convertIntegerToChar(SortingSpeed-1));



    Button(tmp, GetScreenHeight()/8 + font*2,
        TimeButtonText, BLUE, NormalSpeed);

    if (NormalSpeed){
        ChangeSpeed('/', SortingSpeed);
        return;
    }


    tmp += MeasureText(TimeButtonText, font) + 20;
    char SpeedPlusButtonText[] = "+";
    Button(tmp, GetScreenHeight()/8 + font*2,
        SpeedPlusButtonText, ORANGE, addSpeed);


    if (addSpeed){
        ChangeSpeed('+', SortingSpeed);
        return;
    }

    tmp += MeasureText(SpeedPlusButtonText, font) + 20;
    char SpeedMinusButtonText[] = "-";
    Button(tmp, GetScreenHeight()/8 + font*2,
        SpeedMinusButtonText, ORANGE, subSpeed);


    if (subSpeed){
        ChangeSpeed('-', SortingSpeed);
        return;
    }


    tmp = (5 * GetScreenWidth()) / 100;
    char SizeButtonText[] = "Size";
    Button(tmp, GetScreenHeight()/8 + font*2,
        SizeButtonText, BLUE, NormalSize);


    if (NormalSize){
        ChangeSize('/', NumOfBars);
        return;
    }



    tmp += MeasureText(SizeButtonText, font) + 20;
    char SizePlusButton[] = "+";
    Button(tmp, GetScreenHeight()/8 + font*2,
        SizePlusButton, ORANGE, addSize);

    if (addSize){
        ChangeSize('+', NumOfBars);
        return;
    }

    tmp += MeasureText(SizePlusButton, font) + 20;
    char SizeMinusButton[] = "-";
    Button(tmp, GetScreenHeight()/8 + font*2,
        SizeMinusButton, ORANGE, subSize);


    if (subSize){
        ChangeSize('-', NumOfBars);
        return;
    }
}

void ChangeSize(char operation, int &value){
    switch (operation)
    {
    case '-':
        if (value > 5){
            value -= 5;
            for (int i = 0; i < 5; i++)
                arr.pop_back();
        }
        break;

    case '+':
        value += 5;
        for (int i = 0; i < 5; i++)
            arr.push_back({GetRandomValue(40, MinWindowHeight-140), NORMAL});
        break;


    default:
        while (NumOfBars > 50){
            NumOfBars--;
            arr.pop_back();
        }

        while (NumOfBars < 50){
            NumOfBars++;
            arr.push_back({GetRandomValue(40, MinWindowHeight-140), NORMAL});
        }
        break;
    }

    for (int i = 0; i < NumOfBars; i++){
        arr[i].second = NORMAL;
    }


    DrawArray(arr);
}

void ChangeSpeed(char operation, int &value){

    switch (operation)
    {
    case '-':
        value = value - 10 > 0 ? value - 5 : value - 1;
        break;

    case '+':
        value += 5;
        break;

    default:
        value = 41;
        break;

    }
}



void ShowEndingScreen(){
    DrawArray(arr);

    ShouldShowStartingScreen = true;
    ShouldShowEndingScreen = false;
}


void Start_Button(float size, float font, char Start[]){
    Button(size, GetScreenHeight()/8 + font*2,
        Start, RAYWHITE, GameShouldStart);

    return;
}

void Selection_Sort_Button(float size, char Selection_Sort_Text[]){
    Color color;
    if (SelectionSortPressed)
        color = BLUE;
    else
        color = GREEN;

    Button(size, GetScreenHeight()/8, Selection_Sort_Text, color,
        SelectionSortPressed);
}

void Insertion_Sort_Button(float size, char Insertion_Sort_Text[]){
    Color color;
    if (InsertionSortPressed)
        color = BLUE;
    else
        color = GREEN;

    Button(size, GetScreenHeight()/8, Insertion_Sort_Text, color,
        InsertionSortPressed);
}

void Bubble_Sort_Button(float size, char Bubble_Sort_Text[]){
    Color color;
    if (BubbleSortPressed)
        color = BLUE;
    else
        color = GREEN;

    Button(size, GetScreenHeight()/8, Bubble_Sort_Text, color,
        BubbleSortPressed);
}

void Merge_Sort_Button(float size, char Merge_Sort_Text[]){
    Color color;
    if (MergeSortPressed)
        color = BLUE;
    else
        color = GREEN;

    Button(size, GetScreenHeight()/8, Merge_Sort_Text, color,
        MergeSortPressed);
}

void Button(float x, float y, char *Text, Color color, bool &state){
    float font = (2.5 * GetScreenWidth()) / 100;
    Rectangle r1 = {
        .x = x,
        .y = y,
        .width = (float) MeasureText(Text, font),
        .height = (float) font,
    };


    if (CheckCollisionPointRec(GetMousePosition(), r1)){

        DrawText(Text, x, y, font, ORANGE);

        if (IsMouseButtonPressed(0)){
            if (state == true)
                state = false;
            else
                state = true;


            if (state != ShouldRandomize && state != GameShouldStart
            && (state != addSize && state != subSize && state != addSpeed
            && state != subSpeed && state != NormalSize && state != NormalSpeed))

                for (bool *i : SortingChoice)
                    if (i != &state)
                        *i = false;
            return;
        }


    } else {
        DrawText(Text, x, y, font, color);
    }

    return;
}


void DrawArray( vector< pair<int, int>> arr){
    // Drawing the array / pillers:
    float BarWidth = (float) GetScreenWidth() / (NumOfBars);


    for (int i = 0; i < NumOfBars; i++){
        Color color = FindColorForPiller(arr[i].second);

        DrawRectangleV (Vector2{(float) i*BarWidth,
            (float) GetScreenHeight() - arr[i].first},
            Vector2{BarWidth, (float) arr[i].first}, color);
    }
}

void RandomizeArray( vector< pair<int, int >> &arr){
    // RandomizeArray
    if (ShouldRandomize){
        for (int i = 0; i < NumOfBars; i++)
            arr[i] = {GetRandomValue(40, MinWindowHeight-200), NORMAL};

        ShouldRandomize = false;
    }
}


Color FindColorForPiller(int pillerState){
    switch (pillerState)
    {
    case SELECTED:
        return GREEN;
        break;

    case COMPARING:
        return RED;
        break;

    case SORTED:
        return GOLD;
        break;

    case MINIMUM:
        return SKYBLUE;
        break;

    case LEFT:
        return SKYBLUE;
        break;

    case RIGHT:
        return BLUE;
        break;

    default:
        return WHITE;
        break;
    }
}


void SortArray(){
    for (bool *state : SortingChoice){
        if (*state == true){
            if (state == &InsertionSortPressed)
                Insertion_Sort(arr);

            else if (state == &SelectionSortPressed)
                Selection_Sort(arr);

            else if (state == &BubbleSortPressed)
                Bubble_Sort(arr);

            else if (state == &MergeSortPressed)
                Merge_Sort(arr);

        }

        *state = false;
    }

    return;
}



// Selection Sort:
void Selection_Sort( vector< pair<int, int>> &arr){

    for (int i = 0; i < NumOfBars; i++){
        arr[i].second = SELECTED;

         pair<int, int> minNum = {arr[i].first, i};
        for (int j = i; j < NumOfBars; j++){
            arr[j].second = COMPARING;
            arr[minNum.second].second = MINIMUM;


            if (minNum.first > arr[j].first){

                if (minNum.second == i)
                    arr[minNum.second].second = SELECTED;
                else if (minNum.second == j)
                    arr[minNum.second].second = COMPARING;
                else
                    arr[minNum.second].second = NORMAL;


                minNum = {arr[j].first, j};

            }


            BeginDrawing();
            ClearBackground(DARKPURPLE);

            DrawArray(arr);


            if (j == i)
                arr[j].second = SELECTED;
            else
                arr[j].second = NORMAL;

            EndDrawing();
        }

         swap(arr[i], arr[minNum.second]);
         arr[i].second = SORTED;

        for (int j = i+1; j < NumOfBars; j++){
            arr[j].second = NORMAL;
        }
    }
}

// Insertion Sort:
void Insertion_Sort( vector< pair<int, int>> &arr){

    arr[0].second = SORTED;
    for (int i = 1; i < arr.size(); i++){
        BeginDrawing();

        ClearBackground(DARKPURPLE);

        arr[i].second = SELECTED;

//            Color color = GREEN;

            int key = arr[i].first;


            // Sorting the arr
            int tmp = 0;
            int j = i-1;
            while (arr[j].first > key && j >= 0){

                // Swap the two pillers
                 swap(arr[j+1].first, arr[j].first);


                // Make the comparing piller
                arr[j].second = COMPARING;


                BeginDrawing();
                ClearBackground(DARKPURPLE);


                DrawArray(arr);

                // Make the Selected piller sorted again (it already was)
                arr[j].second = SORTED;

                EndDrawing();

                j--;
                tmp++;
            }


            if (tmp == 0){
                BeginDrawing();


                ClearBackground(DARKPURPLE);

                DrawArray(arr);

                EndDrawing();
            }


            // DE-Select the unnecesserry pillers
            for (int k = 0; k < NumOfBars; k++)
                if (arr[k].second == SELECTED)
                    arr[k].second = NORMAL;


            // Color the sorted pillers
            for (int k = i-1; k >= 0; k--)
                arr[k].second = SORTED;


            arr[j+1].first = key;
            arr[j+1].second = SORTED;

        EndDrawing();
    }

    arr[NumOfBars-1].second = SORTED;
}


// Bubble Sort:
void Bubble_Sort( vector< pair<int, int>> &arr){

    int endingPoint = NumOfBars;

    bool swapped;
    do {
        
        swapped = false;

        for (int i = 0; i < endingPoint-1; i++){

            arr[i].second = SELECTED;

            if (arr[i].first > arr[i+1].first){
                // arr[i+1].second = SELECTED;

                 swap(arr[i], arr[i+1]);

                swapped = true;

            }



        BeginDrawing();

        ClearBackground(DARKPURPLE);


            for (int k = NumOfBars-1; k >= endingPoint; k--)
                arr[k].second = SORTED;

            DrawArray(arr);

            for (int k = i; k >= 0; k--)
                arr[k].second = NORMAL;




        EndDrawing();

        }


        endingPoint--;

    } while (swapped);

    for (int k = NumOfBars-1; k >= 0; k--)
        arr[k].second = SORTED;
}

// Merge Sort:

void Merge_Sort( vector< pair<int, int>> &arr){
    MergeSort_Finale(arr, 0, NumOfBars-1);
}

void MergeSort_Finale( vector< pair<int, int>> &arr, int leftIndex, int rightIndex){

    if (leftIndex >= rightIndex)
        return;

    int m = (leftIndex + rightIndex) / 2;

    MergeSort_Finale(arr, leftIndex, m);
    MergeSort_Finale(arr, m+1, rightIndex);

    Merge(arr, leftIndex, m, rightIndex);
}

void Merge( vector< pair<int, int>> &arr,
    int leftIndex, int m, int rightIndex){

    int arrSizeLeft = m - leftIndex + 1;
    int arrSizeRight = rightIndex - m;



    int left[arrSizeLeft];
    for (int i = 0; i < arrSizeLeft; i++){
        left[i] = arr[leftIndex + i].first;
        arr[leftIndex + i].second = LEFT;
    }


    int right[arrSizeRight];
    for (int i = 0; i < arrSizeRight; i++){
        right[i] = arr[m + 1 + i].first;
        arr[m + 1 + i].second = RIGHT;
    }



    BeginDrawing();
        ClearBackground(DARKPURPLE);


        DrawArray(arr);

    EndDrawing();





    int i = 0; // Left index
    int j = 0; // Right index

    int k = leftIndex; // New array index;


    while (i < arrSizeLeft && j < arrSizeRight){
        if (left[i] <= right[j]){
            arr[k].first = left[i];
            i++;
        } else{
            arr[k].first = right[j];
            j++;
        }
        arr[k].second = SORTED;

        k++;
    }


    while(i < arrSizeLeft){
        arr[k].first = left[i];
        arr[k].second = SORTED;

        i++;
        k++;
    }


    while(j < arrSizeRight){
        arr[k].first = right[j];
        arr[k].second = SORTED;

        j++;
        k++;
    }
}