#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h> // لتمكين استخدام دالة atoi()
#include <string.h>

int main(int argc, string argv[])
{
    // المرحلة الأولى: التحقق من المفتاح وتجهيزه

    // 1. التحقق من عدد الوسائط: يجب أن يكون هناك وسيط واحد فقط (argc = 2)
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1; // خروج مع رمز خطأ
    }

    // 2. التحقق من أن المفتاح رقمي بالكامل
    for (int i = 0, len = strlen(argv[1]); i < len; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1; // خروج مع رمز خطأ
        }
    }

    // 3. تحويل المفتاح من نص (string) إلى عدد صحيح (int)
    int key = atoi(argv[1]);

    // المرحلة الثانية: الحصول على النص الأصلي وتطبيقه التشفير

    // 4. طلب النص الأصلي من المستخدم
    string plaintext = get_string("plaintext: ");

    // 5. طباعة كلمة "ciphertext" ثم البدء بالتشفير
    printf("ciphertext: ");

    // 6. تطبيق الإزاحة على كل حرف
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char c = plaintext[i];

        if (isalpha(c)) // هل الحرف أبجدي؟
        {
            if (isupper(c)) // إذا كان حرفاً كبيراً (A-Z)
            {
                // صيغة التشفير: (الحرف - Base + المفتاح) % 26 + Base
                // التحويل إلى index (0-25) ثم الإزاحة والعودة الدائرية
                char ciphertext_char = (char) (((c - 'A') + key) % 26) + 'A';
                printf("%c", ciphertext_char);
            }
            else if (islower(c)) // إذا كان حرفاً صغيراً (a-z)
            {
                // نفس الصيغة للحروف الصغيرة
                char ciphertext_char = (char) (((c - 'a') + key) % 26) + 'a';
                printf("%c", ciphertext_char);
            }
        }
        else // إذا لم يكن الحرف أبجدياً (رمز، مسافة، رقم)
        {
            printf("%c", c); // طباعة الحرف كما هو
        }
    }

    // المرحلة الثالثة: الإنهاء

    printf("\n"); // إضافة سطر جديد بعد النص المشفر
    return 0;     // إنهاء البرنامج بنجاح
}
