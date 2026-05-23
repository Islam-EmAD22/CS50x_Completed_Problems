// Modifies the volume of an audio file

#include <stdint.h> // لـ int16_t (حجم العينات الصوتية)
#include <stdio.h>
#include <stdlib.h>

// حجم الـ Header في ملفات WAV هو 44 بايت دائماً
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // فحص وسائط سطر الأوامر (تأكد من وجود 4 وسائط: اسم البرنامج، ملف الإدخال، ملف الإخراج، ومعامل
    // الصوت)
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // فتح ملف الإدخال للقراءة
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // فتح ملف الإخراج للكتابة
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // تحويل معامل مستوى الصوت (factor) من نص إلى رقم عشري (float)
    float factor = atof(argv[3]);

    // -----------------------------------------------------------------
    // TODO: Copy header from input file to output file (نسخ الـ Header)
    // -----------------------------------------------------------------

    // 1. تعريف مصفوفة (Array) لتخزين الـ Header (44 بايت)
    uint8_t header[HEADER_SIZE];

    // 2. قراءة الـ Header من ملف الإدخال:
    //    (header, HEADER_SIZE, 1, input) تعني: اقرأ 1 عنصر بحجم 44 بايت من 'input' وخزنها في
    //    'header'
    fread(header, HEADER_SIZE, 1, input);

    // 3. كتابة الـ Header في ملف الإخراج:
    //    اكتب 1 عنصر بحجم 44 بايت من 'header' إلى 'output'
    fwrite(header, HEADER_SIZE, 1, output);

    // -----------------------------------------------------------------
    // TODO: Read samples from input file and write updated data to output file (تعديل العينات)
    // -----------------------------------------------------------------

    // تحديد نوع البيانات للعينة الصوتية (Sample)
    // العينات في ملفات WAV تكون غالباً بحجم 16 بت (2 بايت)، ونستخدم int16_t
    int16_t sample;

    // البدء في حلقة تكرار للقراءة حتى نهاية الملف (End Of File - EOF)
    // fread سترجع القيمة 0 عندما تصل إلى نهاية الملف (أي لم تستطع قراءة أي عنصر)
    while (fread(&sample, sizeof(int16_t), 1, input))
    {
        // 1. تعديل مستوى الصوت (الضرب في العامل)
        // يتم تطبيق عامل مستوى الصوت على العينة التي تم قراءتها
        sample = sample * factor;

        // 2. كتابة العينة المُعدَّلة في ملف الإخراج
        // نكتب 1 عنصر بحجم 2 بايت من عنوان 'sample' إلى 'output'
        fwrite(&sample, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);

    return 0;
}
