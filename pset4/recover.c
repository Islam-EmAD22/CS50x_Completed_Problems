#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// تعريف حجم الكتلة الواحدة كما ذكرت المواصفات
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // 1. التأكد من إدخال اسم ملف واحد فقط في سطر الأوامر
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // 2. محاولة فتح ملف البطاقة للقراءة
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open file %s.\n", argv[1]);
        return 1;
    }

    // تعريف المخزن المؤقت (Buffer) بحجم 512 بايت
    uint8_t buffer[BLOCK_SIZE];

    // متغيرات لتتبع الصور
    FILE *img_ptr = NULL; // مؤشر لملف الصورة الحالية
    char filename[8];     // مصفوفة لتخزين اسم الملف (###.jpg + \0)
    int img_count = 0;    // العداد الذي سيعرفنا كم صورة وجدنا

    // 3. قراءة الملف كتلة بكتلة (كل كتلة 512 بايت)
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // 4. هل هذه بداية صورة JPEG جديدة؟
        // فحص البايتات الثلاثة الأولى، والبتات الأربعة الأولى من البايت الرابع
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // إذا كان هناك ملف صورة مفتوح أصلاً، يجب إغلاقه قبل فتح الجديد
            if (img_ptr != NULL)
            {
                fclose(img_ptr);
            }

            // إنشاء اسم الملف الجديد بناءً على العداد (000.jpg, 001.jpg, ...)
            sprintf(filename, "%03i.jpg", img_count);

            // فتح ملف جديد للكتابة
            img_ptr = fopen(filename, "w");

            // زيادة العداد للصورة القادمة
            img_count++;
        }

        // 5. إذا وجدنا صورة وبدأنا الكتابة، استمر في صب البايتات في الملف المفتوح
        if (img_ptr != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, img_ptr);
        }
    }

    // 6. إغلاق الملفات المتبقية عند الوصول لنهاية البطاقة
    if (img_ptr != NULL)
    {
        fclose(img_ptr);
    }

    fclose(raw_file);

    return 0;
}
