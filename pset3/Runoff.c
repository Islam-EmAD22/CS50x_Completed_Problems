#include <cs50.h>
#include <stdio.h>
#include <string.h> // يجب إضافة هذه المكتبة لاستخدام strcmp // This library is needed for strcmp

// الحد الأقصى للناخبين والمرشحين
// Maximum limits for voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] هي التفضيل j للناخب i
// preferences[i][j] is the j-th preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// المرشحون لديهم اسم، عدد أصوات، وحالة إقصاء
// Candidates have a name, vote count, and elimination status
typedef struct
{
    string name;
    int votes;
    bool eliminated; // حالة الإقصاء: true تعني تم إقصاؤه // Elimination status: true means eliminated
} candidate;

// مصفوفة المرشحين
// Array of candidates
candidate candidates[MAX_CANDIDATES];

// أعداد الناخبين والمرشحين
// Voter and candidate counts
int voter_count;
int candidate_count;

// الإعلان عن الدوال
// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // التحقق من الاستخدام غير الصالح (لم يتم إدخال مرشحين)
    // Check for invalid usage (no candidates entered)
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // تعبئة مصفوفة المرشحين
    // Populate the candidates array
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false; // افتراضياً، لم يتم إقصاء أحد // Default: no one is eliminated
    }

    // الحصول على عدد الناخبين
    // Get the number of voters
    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // استمرار طلب التصويت من الناخبين
    // Continue prompting voters for their preferences
    for (int i = 0; i < voter_count; i++)
    {

        // طلب التفضيلات لكل رتبة (1، 2، 3...)
        // Prompt for preferences for each rank (1, 2, 3...)
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // تسجيل التصويت ما لم يكن غير صالح
            // Record the vote unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // استمرار إجراء جولات الإعادة حتى يوجد فائز
    // Continue running runoff rounds until a winner is found
    while (true)
    {
        // حساب الأصوات بناءً على المرشحين المتبقين
        // Tally votes based on remaining candidates
        tabulate();

        // التحقق مما إذا تم الفوز في الانتخابات
        // Check if an election winner has been found
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // إقصاء مرشحي المركز الأخير
        // Eliminate the last-place candidate(s)
        int min = find_min();
        bool tie = is_tie(min);

        // إذا كان هناك تعادل شامل بين الجميع، فالكل فائز
        // If there is an overall tie among all remaining candidates, everyone is a winner
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // إقصاء أي شخص لديه الحد الأدنى من الأصوات
        // Eliminate anyone who has the minimum number of votes
        eliminate(min);

        // تصفير عدد الأصوات استعداداً للجولة التالية
        // Reset vote counts in preparation for the next round
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// تسجيل التفضيل إذا كان التصويت صالحًا
// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // البحث الخطي: حلقة تمر على المرشحين لمقارنة الاسم المدخل
    // Linear Search: Loop through candidates to compare the entered name
    for (int i = 0; i < candidate_count; i++)
    {
        // استخدام strcmp للمقارنة الاسم (تطابق يعني 0)
        // Use strcmp to compare the name (0 means a match)
        if (strcmp(name, candidates[i].name) == 0)
        {
            // 1. تسجيل القيمة الصحيحة:
            // 1. Record the correct value:
            // تخزين فهرس المرشح (i) في مصفوفة التفضيلات لهذا الناخب (voter) والرتبة (rank)
            // Store the candidate's index (i) in the preferences array for this voter (voter) and rank (rank)
            preferences[voter][rank] = i;

            // 2. إرجاع القيمة: إرجاع true عند العثور على تطابق
            // 2. Return value: Return true upon finding a match
            return true;
        }
    }
    // إذا انتهت الحلقة ولم يتم العثور على اسم
    // If the loop ends and no name was found
    return false;
}

// 2. تجميع الأصوات للمرشحين غير المقصيين
// 2. Tally votes for non-eliminated candidates
void tabulate(void)
{
    // تصفير الأصوات في بداية كل جولة (تم نقل هذه الخطوة إلى الدالة main لتبسيط التصفير)
    // Reset votes at the start of each round (this step was moved to main for simpler reset)

    // المرور على كل ناخب
    // Loop through every voter
    for (int i = 0; i < voter_count; i++)
    {
        // المرور على تفضيلات الناخب بالترتيب (من الأول j=0)
        // Loop through the voter's preferences in order (starting from the first j=0)
        for (int j = 0; j < candidate_count; j++)
        {
            // الحصول على فهرس المرشح من التفضيل الحالي للناخب i في الرتبة j
            // Get the candidate's index from the current preference of voter i at rank j
            int candidate_index = preferences[i][j];

            // التحقق مما إذا كان المرشح لم يُقصَ
            // Check if the candidate has NOT been eliminated
            if (!candidates[candidate_index].eliminated)
            {
                // منح الصوت والانتقال للناخب التالي
                // Give the vote and move to the next voter
                candidates[candidate_index].votes++;
                break;
            }
        }
    }
    return;
}

// 3. طباعة الفائز في الانتخابات، إذا وُجد
// 3. Print the election winner, if found
bool print_winner(void)
{
    int majority = voter_count / 2; // حساب الأغلبية المطلقة // Calculate the absolute majority

    // البحث عن مرشح أصواته > الأغلبية
    // Search for a candidate whose votes > majority
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > majority)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// 4. إرجاع الحد الأدنى لعدد الأصوات لأي مرشح متبقٍ
// 4. Return the minimum vote count for any remaining candidate
int find_min(void)
{
    int min_votes = MAX_VOTERS + 1; // قيمة أكبر من أي عدد أصوات ممكن // A value larger than any possible vote count

    // البحث عن أقل عدد أصوات بين المرشحين غير المقصيين
    // Search for the minimum vote count among non-eliminated candidates
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated) // إذا كان المرشح متبقياً // If the candidate remains
        {
            if (candidates[i].votes < min_votes)
            {
                min_votes = candidates[i].votes;
            }
        }
    }
    return min_votes;
}

// 5. إرجاع true إذا كانت الانتخابات متعادلة بين كل المرشحين، و false بخلاف ذلك
// 5. Return true if the election is tied among ALL candidates, and false otherwise
bool is_tie(int min)
{
    // التحقق مما إذا كان جميع المرشحين غير المقصيين لديهم min من الأصوات
    // Check if ALL non-eliminated candidates have 'min' votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated) // إذا كان المرشح متبقياً // If the candidate remains
        {
            // إذا وُجد أي مرشح متبقٍ أصواته لا تساوي min، فليس تعادلاً شاملاً
            // If any remaining candidate's votes do NOT equal 'min', it is NOT an overall tie
            if (candidates[i].votes != min)
            {
                return false;
            }
        }
    }
    // إذا لم نجد أحداً أصواته تختلف عن min (بين المتبقين)، فهو تعادل شامل
    // If we didn't find anyone whose votes differ from 'min' (among the remaining), it is an overall tie
    return true;
}

// 6. إقصاء المرشح (أو المرشحين) في المركز الأخير
// 6. Eliminate the last-place candidate(s)
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true; // الإقصاء // Elimination
        }
    }
    return;
}
