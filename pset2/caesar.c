#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h> // لتمكين استخدام دالة atoi() / Enable atoi()
#include <string.h>

int main(int argc, string argv[])
{
    // المرحلة الأولى: التحقق من المفتاح وتجهيزه
    // Phase 1: Key Validation and Preparation

    // 1. التحقق من عدد الوسائط: يجب أن يكون هناك وسيط واحد فقط (argc = 2)
    // 1. Check argument count: Must have exactly one argument (argc = 2)
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1; // خروج مع رمز خطأ / Exit with error code
    }

    // 2. التحقق من أن المفتاح رقمي بالكامل
    // 2. Check if the key is purely numeric
    for (int i = 0, len = strlen(argv[1]); i < len; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1; // خروج مع رمز خطأ / Exit with error code
        }
    }

    // 3. تحويل المفتاح من نص (string) إلى عدد صحيح (int)
    // 3. Convert the key from string to an integer
    int key = atoi(argv[1]);

    // المرحلة الثانية: الحصول على النص الأصلي وتطبيقه التشفير
    // Phase 2: Get plaintext and apply encryption

    // 4. طلب النص الأصلي من المستخدم
    // 4. Prompt user for plaintext
    string plaintext = get_string("plaintext: ");

    // 5. طباعة كلمة "ciphertext" ثم البدء بالتشفير
    // 5. Print "ciphertext" prefix before encrypting
    printf("ciphertext: ");

    // 6. تطبيق الإزاحة على كل حرف
    // 6. Apply the shift to every character
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char c = plaintext[i];

        if (isalpha(c)) // هل الحرف أبجدي؟ / Is the character alphabetic?
        {
            if (isupper(c)) // إذا كان حرفاً كبيراً (A-Z) / If it is an uppercase letter (A-Z)
            {
                // صيغة التشفير: (الحرف - Base + المفتاح) % 26 + Base
                // التحويل إلى index (0-25) ثم الإزاحة والعودة الدائرية
                // Cipher Formula: (Char - Base + Key) % 26 + Base
                // Convert to 0-25 index, apply shift, and wrap around
                char ciphertext_char = (char) (((c - 'A') + key) % 26) + 'A';
                printf("%c", ciphertext_char);
            }
            else if (islower(c)) // إذا كان حرفاً صغيراً (a-z) / If it is a lowercase letter (a-z)
            {
                // نفس الصيغة للحروف الصغيرة / Same formula for lowercase
                char ciphertext_char = (char) (((c - 'a') + key) % 26) + 'a';
                printf("%c", ciphertext_char);
            }
        }
        else // إذا لم يكن الحرف أبجدياً (رمز، مسافة، رقم) / If the character is non-alphabetic
        {
            printf("%c", c); // طباعة الحرف كما هو / Print the character as is
        }
    }

    // المرحلة الثالثة: الإنهاء
    // Phase 3: Termination

    printf("\n"); // إضافة سطر جديد بعد النص المشفر / Add a newline after the ciphertext
    return 0;      // إنهاء البرنامج بنجاح / Terminate the program successfully
}
