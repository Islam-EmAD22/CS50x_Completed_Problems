#include <cs50.h>
#include <stdio.h>
#include <string.h> // لاستخدام دالة strcmp / For using strcmp function

// الحدود القصوى: 9 مرشحين كحد أقصى
// Maximum limits: 9 candidates maximum
#define MAX_CANDIDATES 9

// 1. هيكل البيانات (Data Structure)
// 1. Data Structure (Struct) definition
typedef struct
{
    string name;
    int votes;
} candidate;

// إعلان المصفوفة عالميًا (Global): لتكون مرئية لدالتي vote و print_winner
// Global array declaration: to be visible to vote and print_winner functions
candidate candidates[MAX_CANDIDATES];

// إعلان عدد المرشحين عالميًا
// Global declaration of candidate count
int candidate_count;

// الإعلان عن الدوال
// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    /* 2. التحقق من المدخلات (المرشحون)
    2. Input validation (Candidates) */
    candidate_count = argc - 1; /* عدد المرشحين هو عدد الوسائط مطروحًا منه اسم البرنامج (argv[0])
    Candidate count is the number of arguments minus the program name (argv[0]) */

    if (argc < 2 || candidate_count > MAX_CANDIDATES)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    /* 3. تخزين المرشحين و تهيئة الأصوات بالصفر
    3. Store candidates and initialize votes to zero */
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1]; /* argv[1] هو أول مرشح / argv[1] is the first candidate */

        /* البدء بعد الأصوات من الصفر
        Start counting votes from zero */
        candidates[i].votes = 0;
    }

    /* 4. الحصول على أصوات الناخبين
    4. Get voter input */
    int voter_count = get_int("Number of voters: ");

    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        /* استدعاء دالة التصويت (الخوارزمية)
        Call the vote function (Algorithm) */
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    /* 5. إعلان الفائز
    5. Declare the winner(s) */
    print_winner();

    return 0;
}

/* الخوارزمية: دالة vote (البحث الخطي والتجميع)
Algorithm: vote function (Linear Search and Accumulation) */
bool vote(string name)
{
    /* البحث الخطي: حلقة للمرور على كل مرشح
    Linear Search: Loop through every candidate */
    for (int i = 0; i < candidate_count; i++)
    {
        /* مقارنة اسم المرشح المُدخل باسم المرشح في المصفوفة
        Compare the input candidate name with the candidate name in the array */
        if (strcmp(name, candidates[i].name) == 0)
        {
            /* التجميع: زيادة عداد الأصوات
            Accumulation: Increment the vote counter */
            candidates[i].votes++;
            return true; /* تم العثور على المرشح وتجميع صوته بنجاح / Candidate found and vote tallied successfully */
        }
    }
    return false; /* لم يتم العثور على المرشح / Candidate not found */
}

/* دالة print_winner (إيجاد الفائز)
print_winner function (Finding the winner) */
void print_winner(void)
{
    /* 1. البحث عن الحد الأقصى للأصوات (max_votes)
    1. Find the maximum number of votes (max_votes) */
    int max_votes = 0;

    /* حلقة أولى لتحديد أكبر عدد من الأصوات
    First loop to determine the highest vote count */
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > max_votes)
        {
            max_votes = candidates[i].votes;
        }
    }

    /* 2. طباعة أسماء جميع الفائزين المتعادلين على max_votes
    2. Print the names of all winners tied at max_votes */
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == max_votes)
        {
            printf("%s\n", candidates[i].name);
        }
    }
    return;
}
