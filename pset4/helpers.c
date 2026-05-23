#include "helpers.h"
#include "math.h"
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int w = 0; w < width; w++)
        {
            int red = image[i][w].rgbtRed;
            int green = image[i][w].rgbtGreen;
            int blue = image[i][w].rgbtBlue;
            float average = (red + green + blue) / 3.0;
            int rounded_average = round(average);
            image[i][w].rgbtRed = rounded_average;
            image[i][w].rgbtGreen = rounded_average;
            image[i][w].rgbtBlue = rounded_average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int s = 0; s < width; s++)
        {
            int red = image[i][s].rgbtRed;
            int green = image[i][s].rgbtGreen;
            int blue = image[i][s].rgbtBlue;
            float sepiaRed = (.393 * red + .769 * green + .189 * blue);
            int rounded_sepiaRed = round(sepiaRed);

            float sepiaGreen = (.349 * red + .686 * green + .168 * blue);
            int rounded_sepiaGreen = round(sepiaGreen);

            float sepiaBlue = (.272 * red + .534 * green + .131 * blue);
            int rounded_sepiaBlue = round(sepiaBlue);

            if (rounded_sepiaRed > 255)
            {
                rounded_sepiaRed = 255;
            }
            if (rounded_sepiaGreen > 255)
            {
                rounded_sepiaGreen = 255;
            }
            if (rounded_sepiaBlue > 255)
            {
                rounded_sepiaBlue = 255;
            }
            image[i][s].rgbtRed = rounded_sepiaRed;
            image[i][s].rgbtGreen = rounded_sepiaGreen;
            image[i][s].rgbtBlue = rounded_sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // نمر حتى نصف العرض فقط
        for (int j = 0; j < width / 2; j++)
        {
            // 1. يجب أن يكون temp من نوع RGBTRIPLE ليحمل البكسل كاملاً
            RGBTRIPLE temp = image[i][j];

            // 2. نستخدم j في الحساب للوصول للبكسل المقابل في نفس الصف
            image[i][j] = image[i][width - 1 - j];

            // 3. نعيد القيمة المخزنة للطرف الآخر
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // 1. إنشاء نسخة من الصورة للأخذ منها (عشان ما نعدل في الأصل ونقرأ منه في نفس الوقت)
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // 2. المرور على كل بكسل في الصورة الأصلية لتعديله
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumRed = 0, sumGreen = 0, sumBlue = 0;
            float counter = 0;

            // 3. البحث عن الجيران في مربع 3x3 (من -1 إلى +1 حول البكسل الحالي)
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int neighbor_i = i + k;
                    int neighbor_j = j + l;

                    // 4. فحص الحدود: هل الجار موجود فعلاً داخل الصورة؟
                    if (neighbor_i >= 0 && neighbor_i < height && neighbor_j >= 0 &&
                        neighbor_j < width)
                    {
                        sumRed += copy[neighbor_i][neighbor_j].rgbtRed;
                        sumGreen += copy[neighbor_i][neighbor_j].rgbtGreen;
                        sumBlue += copy[neighbor_i][neighbor_j].rgbtBlue;
                        counter++;
                    }
                }
            }

            // 5. حساب المتوسط ووضعه في الصورة الأصلية
            image[i][j].rgbtRed = round(sumRed / counter);
            image[i][j].rgbtGreen = round(sumGreen / counter);
            image[i][j].rgbtBlue = round(sumBlue / counter);
        }
    }
    return;
}
