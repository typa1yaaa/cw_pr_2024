#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define MAX_FILENAME 500
#define MAX_CONST 100
#define INFO_ABOUT_STUD "Course work for option 4.3, created by Ksenia Kopasova.\n"

#pragma pack(push, 1)

typedef struct BitmapFileHeader {
    unsigned short signature;
    unsigned int filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int pixelArrOffset;
} BitmapFileHeader;

typedef struct BitmapInfoHeader {
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    unsigned int xPixelsPerMeter;
    unsigned int yPixelsPerMeter;
    unsigned int colorsInColorTable;
    unsigned int importantColorCount;
} BitmapInfoHeader;

typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Rgb;

//#pragma pack(pop)

double fmod(double x, double y){
    if (y==0.0){
        return 0.0/0.0;
    }
    double res=x;
    while (res>=y){
        res-=y;
    }
    while (res<0){
        res+=y;
    }
    return res;
}

int roundd(double num){
    if (num<0){
        return (int)(num-0.5);

    }else{
        return (int)(num+0.5);

    }
}


struct option long_options[] = {

    {"rect", no_argument, NULL, 'e'},
    {"rotate", no_argument, NULL, 'r'},
    {"input", required_argument, NULL, 'i'},
    {"output", required_argument, NULL, 'o'},
    {"info", no_argument, NULL, 'n'},
    {"help", no_argument, NULL, 'h'},
///
    {"bitwise_threshold", no_argument, NULL, 'b'},
    {"op", required_argument, NULL, 20},
    {"threshold_red", required_argument, NULL, 21},
    {"threshold_blue", required_argument, NULL, 22},
    {"threshold_green", required_argument, NULL, 23},
///
    {"left_up", required_argument, NULL, 1},
    {"right_down", required_argument, NULL, 2},
    {"thickness", required_argument, NULL, 3},
    {"color", required_argument, NULL, 4},
    {"fill", no_argument, NULL, 5},
    {"fill_color", required_argument, NULL, 6},
    {"ornament", no_argument, NULL, 10},
    {"pattern", required_argument, NULL, 7},
    {"count", required_argument, NULL, 8},
    {"angle", required_argument, NULL, 9},

    {NULL, 0, NULL, 0}
};

Rgb** read_bmp(const char filename[], BitmapFileHeader* bmfh, BitmapInfoHeader* bmif);
void write_bmp(const char file_name[], Rgb **arr, int H, int W, BitmapFileHeader* bmfh, BitmapInfoHeader* bmif);

void print_file_header(BitmapFileHeader header);
void print_info_header(BitmapInfoHeader header);
void printHelp();

void DrawRectOrn(Rgb** file_bmp, unsigned int H, unsigned int W, int x0, int y0, int x1, int y1, int thickness, int* color, int fill_flag, int* fill_color, int FLAG);
void swap_int(int* a, int* b);
void draw_line(Rgb **arr, int H, int W, int x0, int y0, int x1, int y1, int thickness, int color[3]);

size_t min(size_t a, size_t b);
size_t max(size_t a, size_t b);

void RotatePict(Rgb** arr, int H, int W, int x0, int y0, int x1, int y1, int angle);

void DrawSemic_OY(Rgb** arr, int H, int W, int xc, int yc, int rad, int thickness, int* color, int count);
void DrawSemic_OX(Rgb** arr, int H, int W, int xc, int yc, int rad, int thickness, int* color, int count);
void drawCircleOrn(Rgb** arr, int W, int H, int x0, int y0, int radius, int thickness, int* color, int fl_cir);

void setPixels(Rgb** arr, int x, int y, int* color);

double poww(double base, double exponent) {
    double result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

int main(int argc, char* argv[]){

    char filename[MAX_FILENAME];
    char output_filename[MAX_FILENAME];
    int thickness = 0;
    int fill_flag = 0;
    int x0 = -1, y0 = -1, x1 = -1, y1 = -1, rrr = 0, ggg = 0, bbb = 0, rrr1 = 0, ggg1 = 0, bbb1 = 0;
    int color[3];
    int fill_color[3];

    char pattern[MAX_CONST];
    int count = 0;
    int flag_orn_pattern = -1;

    int angle = 0;

    int flag_rect = 0;
    int flag_ornam = 0;
    int flag_rotate = 0;
    int flag_info = 0;

    int opt;
    int option_index = 0;

    //
    int flag_bitwise_threshold = 0;
    char opttt[MAX_CONST];
    int flag_opt = -1;

    int threshold_red = 0;
    int threshold_blue = 0;
    int threshold_green = 0;

    //

    opt = getopt_long_only(argc, argv, "i:o:h:r:e:n:b:", long_options, &option_index);
    while (opt != -1) {
        switch (opt) {

            ////
            case 'b':{//rect
                flag_bitwise_threshold = 1;
                break;
            }
            case 20:{//rect
                strncpy(opttt, optarg, MAX_CONST - 1);

                if (strcmp(opttt, "and") == 0){
                    flag_opt = 0;
                }
                if (strcmp(opttt, "xor") == 0){
                    flag_opt = 1;
                }
                break;
            }
            
            case 21:{
                sscanf(optarg, "%d", &threshold_red);
                break;
            }
            case 22:{
                sscanf(optarg, "%d", &threshold_blue);
                break;
            }
            case 23:{
                sscanf(optarg, "%d", &threshold_green);
                break;
            }
            ///

            case 'e':{//rect
                flag_rect = 1;
                printf("%d", flag_rect);
                break;
            }

            case 10:{//ornament
                flag_ornam = 1;
                break;
            }

            case 7:{//pattern
                strncpy(pattern, optarg, MAX_CONST - 1);

                if (strcmp(pattern, "rectangle") == 0){
                    flag_orn_pattern = 0;
                }
                if (strcmp(pattern, "circle") == 0){
                    flag_orn_pattern = 1;
                }
                if (strcmp(pattern, "semicircles") == 0){
                    flag_orn_pattern = 2;
                }

                if (flag_orn_pattern == -1){
                    printf("error 209\n");
                    exit(41);
                }

                break;
            }

            case 8:{//count 
                sscanf(optarg, "%d", &count);
                if (count <= 0){
                    printf("error 219\n");
                    exit(41);
                }   
                break;
            }

            case 'r':{//rotate
                flag_rotate = 1;
                break;
            }

            case 9:{//rotate
                sscanf(optarg, "%d", &angle);
                if (angle != 90 && angle != 180 && angle != 270){
                    printf("error 233\n");
                    exit(41);
                }
                break;
            }

            case 'n':{//info
                flag_info = 1;
                break;
            }

            case 'i':{//задаем имя входного изображения
                strncpy(filename, optarg, MAX_FILENAME-1);
                if (filename == NULL){
                    printf("error 247\n");
                    exit(41);
                }
                break;
            }

            case 'o':{//задаем имя выходного изображения
                strncpy(output_filename, optarg, MAX_FILENAME-1);
                if (output_filename == NULL){
                    printf("error, 256\n");
                    exit(41);
                }

                break;
            }
            case 1:{//координаты левого верхнего угла. задаем left.up - x.y. прописать return
                sscanf(optarg, "%d.%d", &x0, &y0);
                if (x0<0 || y0<0){
                    printf("error 265\n");
                    exit(42);
                }

                break;
            }
            case 2:{//координаты правого нижнего угла. задаем right.up - x.y
                sscanf(optarg, "%d.%d", &x1, &y1);
                if (x1<0 || y1<0){
                    printf("error 274\n");
                    exit(42);
                }
                break;
            }
            case 3:{//толщина линий. на вход число больше 0.
                thickness = atoi(optarg);
                if (thickness <= 0){
                    printf("error 282\n");
                    exit(42);
                }
                break;
            }
            case 4:{//color
                sscanf(optarg, "%d.%d.%d", &rrr, &ggg, &bbb);
                color[0] = rrr;
                color[1] = ggg;
                color[2] = bbb;
                if ((rrr<0 || rrr>255) || (ggg<0 || ggg>255) || (bbb<0 || bbb>255)){
                    printf("error 293\n");
                    exit(42);
                }
                break;
            }
            case 5:{//залит или нет
                fill_flag = 1;
                break;
            }
            case 6:{//цвет заливки если он есть
                sscanf(optarg, "%d.%d.%d", &rrr1, &ggg1, &bbb1);
                fill_color[0] = rrr1;
                fill_color[1] = ggg1;
                fill_color[2] = bbb1;
                if ((rrr1<0 || rrr1>255) || (ggg1<0 || ggg1>255) || (bbb1<0 || bbb1>255)){
                    printf("error 308\n");
                    exit(42);
                }
                break;
            }
            case 'h':{//справка
                printf(INFO_ABOUT_STUD);
                printHelp();
                break;
            }

        }
        opt = getopt_long_only(argc, argv, "i:o:h:r:e:n:b:", long_options, &option_index);
    }

    argc -= optind;
    argv += optind;

    if (strcmp(filename, output_filename) == 0){
        printf("error 327\n");
        exit(42);
    }

    if (flag_info){//print info
        printf(INFO_ABOUT_STUD);

        BitmapFileHeader bmfh;
        BitmapInfoHeader bmif;

        //Rgb** file_bmp = read_bmp(filename, &bmfh, &bmif);
        print_file_header(bmfh);
        print_info_header(bmif);

        return 0;
    }

    ////
    if (flag_bitwise_threshold){
        printf(INFO_ABOUT_STUD);

        BitmapFileHeader bmfh;
        BitmapInfoHeader bmif;

        Rgb** file_bmp = read_bmp(filename, &bmfh, &bmif);

        unsigned int H = bmif.height;
        unsigned int W = bmif.width;
        int res_comp = 0;
        for (int x=0; x<W; x++){
            for (int y=0; y<H; y++){
                
                if (flag_opt == 0){
                    res_comp = file_bmp[y][x].r & file_bmp[y][x].g & file_bmp[y][x].b;
                }else if (flag_opt == 1){
                    res_comp = file_bmp[y][x].r ^ file_bmp[y][x].g ^ file_bmp[y][x].b;
                }
                if (res_comp > threshold_red){
                    file_bmp[y][x].r = res_comp;
                }else{
                    file_bmp[y][x].r = 0;
                }
                if (res_comp > threshold_blue){
                    file_bmp[y][x].b = res_comp;
                }else{
                    file_bmp[y][x].b = 0;
                }
                if (res_comp > threshold_green){
                    file_bmp[y][x].g = res_comp;
                }else{
                    file_bmp[y][x].g = 0;
                }
            }
        }
        write_bmp(output_filename, file_bmp, H, W, &bmfh, &bmif);
    }
    ////

    if (flag_rect){//рисуем прямоугольнички
        printf(INFO_ABOUT_STUD);

        BitmapFileHeader bmfh;
        BitmapInfoHeader bmif;

        Rgb** file_bmp = read_bmp(filename, &bmfh, &bmif);
        unsigned int H = bmif.height;
        unsigned int W = bmif.width;
        if (x0>W || y0>H || x1>W || y1>H || thickness == 0 ){        
            printf("error 377\n");
            exit(41);
        }

        DrawRectOrn(file_bmp, H, W, x0, y0, x1, y1, thickness, color, fill_flag, fill_color, 1);

        int rad = thickness/2;
        //сомнительно но окей, скругляем 
        drawCircleOrn(file_bmp, H, W, x0, H-y0, rad, 1, color, 2);
        drawCircleOrn(file_bmp, H, W, x0, H-y1, rad, 1, color, 2);
        drawCircleOrn(file_bmp, H, W, x1, H-y1, rad, 1, color, 2);
        drawCircleOrn(file_bmp, H, W, x1, H-y0, rad, 1, color, 2);

        write_bmp(output_filename, file_bmp, H, W, &bmfh, &bmif);
    }

    if (flag_ornam){//рисуем рамочки
        printf(INFO_ABOUT_STUD);

        BitmapFileHeader bmfh;
        BitmapInfoHeader bmif;

        Rgb** file_bmp = read_bmp(filename, &bmfh, &bmif);
        unsigned int H = bmif.height;
        unsigned int W = bmif.width;

        if (flag_orn_pattern==0){//рисуем прямоугольную рамочку
            int start_x = 0;
            int start_y = 0;
            int y2 = H;
            int x2 = W;

            if (thickness == 0 || count == 0){
                printf("error 410\n");
                exit(41);
            }

            while (count > 0){
                DrawRectOrn(file_bmp, H, W, start_x, start_y, x2, y2, thickness, color, fill_flag, fill_color, 0);
                start_x += 2*thickness;
                start_y += 2*thickness;
                x2 -= 2*thickness;
                y2 -= 2*thickness;
                count--;
            }
        }

        if (flag_orn_pattern==1){//рисуем рамочку-кружочек
            int yc = H/2;
            int xc = W/2;
            int radius = 0;

            if (xc <= yc){
                radius = xc;
            }else{
                radius = yc;
            }

            drawCircleOrn(file_bmp, H, W, xc, yc, radius, 1, color, 1);
            if (radius == yc){
                draw_line(file_bmp, H, W, xc-radius, yc+radius+2, xc-radius, yc-radius, xc-radius+1, color);
                draw_line(file_bmp, H, W, W, H+1, W, 0, xc-radius, color);
            }else{
                draw_line(file_bmp, H, W, 0, yc-radius, xc+radius, yc-radius, yc-radius, color);
                draw_line(file_bmp, H, W, W, H, 0, H, yc-radius, color);
            }
        }

        if (flag_orn_pattern==2){//рамка с полукругами
            if (thickness == 0 || count == 0){
                printf("error 447\n");
                exit(41);
            }
            int new_thick = thickness;
            int rad_OY, rad_OX, d_OY, d_OX;
            if (H%count){
                d_OY = H/count + 1;
            }else{
                d_OY = H/count;
            }
            if (d_OY%2){
                rad_OY = d_OY/2 + 1;
            }else{
                rad_OY = d_OY/2;
            }

            if (W%count){
                d_OX = W/count + 1;
            }else{
                d_OX = W/count;
            }

            if (d_OX%2){
                rad_OX = d_OX/2 + 1;
            }else{
                rad_OX = d_OX/2;
            }

            if (thickness%2){
                new_thick++;
            }

            DrawSemic_OY(file_bmp, H, W, 0, H - rad_OY , rad_OY + thickness/2, thickness, color, count);
            DrawSemic_OY(file_bmp, H, W, W-1, H - rad_OY , rad_OY + thickness/2, thickness, color, count);
            DrawSemic_OX(file_bmp, H, W, rad_OX - thickness/2 + 5, 0, rad_OX+thickness/2, thickness, color, count);
            DrawSemic_OX(file_bmp, H, W, rad_OX - thickness/2 + 5, H - 1, rad_OX+thickness/2, thickness, color, count);
        }

        write_bmp(output_filename, file_bmp, H, W, &bmfh, &bmif);
    }

    if (flag_rotate){//поворачиваем часть изображения
        printf(INFO_ABOUT_STUD);

        BitmapFileHeader bmfh;
        BitmapInfoHeader bmif;

        Rgb** file_bmp = read_bmp(filename, &bmfh, &bmif);
        unsigned int H = bmif.height;
        unsigned int W = bmif.width;

        RotatePict(file_bmp, H, W, x0, y0, x1, y1, angle);

        write_bmp(output_filename, file_bmp, H, W, &bmfh, &bmif);
    }

    return 0;
}

size_t max(size_t a, size_t b){
    if(a>b){
        return a;
    }else{
        return b;
    }
}

size_t min(size_t a, size_t b){
    if(a<b){
        return a;
    }else{
        return b;
    }
}

void DrawSemic_OY(Rgb** arr, int H, int W, int xc, int yc, int rad, int thickness, int* color, int count){
    int c = count;
    while (c>0){
        drawCircleOrn(arr, H, W, xc, yc, rad, thickness, color, 0);
        yc -= 2*rad - thickness ;
        c--;
    }
}

void DrawSemic_OX(Rgb** arr, int H, int W, int xc, int yc, int rad, int thickness, int* color, int count){
    int c = count;
    while (c>0){
        drawCircleOrn(arr, H, W, xc, yc, rad, thickness, color, 0);
        xc += 2*rad - thickness;
        c--;
    }
}


void RotatePict(Rgb** arr, int H, int W, int x0, int y0, int x1, int y1, int angle){
    int height = abs(y1 - y0);
    int width = abs(x1 - x0);
    int xc = (x0+x1)/2;
    int yc = (y0+y1)/2;

    int start_x = xc - height/2;
    int start_y = H - (yc - width/2) - 1;
    y0 = H - y0 - 1;
    y1 = H - y1 - 1;

    int max_x = max(x0, x1);
    int min_x = min(x0, x1);
    int max_y = max(y0, y1);
    int min_y = min(y0, y1);

    int pix_col[3];

    Rgb** copy_color = (Rgb**)malloc(height * sizeof(Rgb*));
    for (int i = 0; i < height; i++) {
        copy_color[i] = (Rgb*)malloc(width * sizeof(Rgb));
        for (int j=0; j<width; j++){
            copy_color[i][j] = arr[min_y + i + 1][min_x + j];
        }
    }

    if (angle == 90){
        for (int y = 0; y < width; y++){
            for (int x = 0; x < height; x++){
                pix_col[0] = copy_color[x][y].r;
                pix_col[1] = copy_color[x][y].g;
                pix_col[2] = copy_color[x][y].b;

                setPixels(arr, start_x + (height - x - 1), start_y - (width - y - 1), pix_col);
            }
        }
    }else if (angle == 180){
        for (int y = max_y; y > min_y; y--){
            for (int x = min_x; x < max_x; x++){
                pix_col[0] = copy_color[(max_y - y)][width - (x - min_x) - 1].r;
                pix_col[1] = copy_color[(max_y - y)][width - (x - min_x) - 1].g;
                pix_col[2] = copy_color[(max_y - y)][width - (x - min_x) - 1].b;

                setPixels(arr, x, y, pix_col);
            }
        }
    }else if (angle == 270){
        for (int y = 0; y < width; y++){
            for (int x = 0; x < height; x++){
                pix_col[0] = copy_color[x][y].r;
                pix_col[1] = copy_color[x][y].g;
                pix_col[2] = copy_color[x][y].b;

                setPixels(arr, start_x + x, start_y - y, pix_col);
            }
        }
    }

    for (int i = 0; i < height; i++){
        free(copy_color[i]);
    }
    free(copy_color);

}

void setPixels(Rgb** arr, int x, int y, int* color) {
    // Установка цвета пикселя в массиве arr по координатам (x, y)
    if (x >=0 && y>=0){    
        arr[y][x].r = color[0];
        arr[y][x].g = color[1];
        arr[y][x].b = color[2];
    }
}

void drawCircleOrn(Rgb** arr, int H, int W, int x0, int y0, int radius, int thickness, int* color, int fl_cir) {
    int t = 0;
    while (t < thickness) {
        int cur_rad = radius - t; // Текущий радиус окружности (увеличивается с толщиной)
        for (int x = x0 - cur_rad; x <= x0 + cur_rad; x++) {
            for (int y = y0 - cur_rad; y <= y0 + cur_rad; y++) {
                if ( x < W  && y < H ) {
                    int dx = (x0 - x);
                    int dy = (y - y0);
                    int sum_sq = dx * dx + dy * dy;

                    if ((sum_sq >= cur_rad * cur_rad) && fl_cir == 1) {
                        // Пиксель находится вне окружности
                        setPixels(arr, x, y, color);
                    }

                    if ((sum_sq >= (cur_rad - 1) * (cur_rad - 1)) && (sum_sq < cur_rad * cur_rad) && fl_cir == 0) {
                        // Пиксель находится на окружности
                        setPixels(arr, x, y, color);
                    }

                    if ((sum_sq < cur_rad * cur_rad) && fl_cir == 2) {
                        // Пиксель в окр
                        setPixels(arr, x, y, color);
                    }
                }
            }
        }
        t++;
    }
}

void DrawRectOrn(Rgb** file_bmp, unsigned int H, unsigned int W, int x0, int y0, int x1, int y1, int thickness, int* color, int fill_flag, int* fill_color, int FLAG){
    if (fill_flag){
            //рисуем и заливаем наш прямоугольник
            int thickness_fill = abs(x1-x0)+thickness/2;

            if (x1>x0){
                if (y0>y1){
                    draw_line(file_bmp, H, W, x1, y1, x1, y0, thickness_fill, fill_color);
                }else{
                    draw_line(file_bmp, H, W, x1, y0, x1, y1, thickness_fill, fill_color);
                }
            }else{
                if (y0>y1){
                    draw_line(file_bmp, H, W, x0, y1, x0, y0, thickness_fill, fill_color);
                }else{
                    draw_line(file_bmp, H, W, x0, y0, x0, y1, thickness_fill, fill_color);
                }
            }

            if (thickness%2==0){
                draw_line(file_bmp, H, W, x0+(thickness/2), y0, x0+(thickness/2), y1, thickness, color);
                draw_line(file_bmp, H, W, x0, y1+(thickness/2)-1, x1, y1+(thickness/2)-1, thickness, color);
                draw_line(file_bmp, H, W, x1+(thickness/2)-1, y1, x1+(thickness/2)-1, y0, thickness, color);
                draw_line(file_bmp, H, W, x0, y0+(thickness/2), x1, y0+(thickness/2), thickness, color);
            }else{
                draw_line(file_bmp, H, W, x0+(thickness/2), y0, x0+(thickness/2), y1, thickness, color);
                draw_line(file_bmp, H, W, x0, y1+(thickness/2), x1, y1+(thickness/2), thickness, color);
                draw_line(file_bmp, H, W, x1+(thickness/2), y1, x1+(thickness/2), y0, thickness, color);
                draw_line(file_bmp, H, W, x0, y0+(thickness/2), x1, y0+(thickness/2), thickness, color);
            }            

        }else{
            //просто рисуем прямоугольник :)
            if (FLAG){//скругляемое
                    if (thickness%2==0){
                    draw_line(file_bmp, H, W, x0+(thickness/2), y0, x0+(thickness/2), y1, thickness, color);
                    draw_line(file_bmp, H, W, x0, y1+(thickness/2)-1, x1, y1+(thickness/2)-1, thickness, color);
                    draw_line(file_bmp, H, W, x1+(thickness/2)-1, y1, x1+(thickness/2)-1, y0, thickness, color);
                    draw_line(file_bmp, H, W, x0, y0+(thickness/2), x1, y0+(thickness/2), thickness, color);
                }else{
                    draw_line(file_bmp, H, W, x0+(thickness/2), y0, x0+(thickness/2), y1, thickness, color);
                    draw_line(file_bmp, H, W, x0, y1+(thickness/2), x1, y1+(thickness/2), thickness, color);
                    draw_line(file_bmp, H, W, x1+(thickness/2), y1, x1+(thickness/2), y0, thickness, color);
                    draw_line(file_bmp, H, W, x0, y0+(thickness/2), x1, y0+(thickness/2), thickness, color);
                }  
            }else{//рамка
                draw_line(file_bmp, H, W, x0+thickness-1, y0+1, x0+thickness-1, y1, thickness, color);
                draw_line(file_bmp, H, W, x0, y1, x1-1, y1, thickness, color);
                draw_line(file_bmp, H, W, x1-1, y0+1, x1-1, y1, thickness, color);
                draw_line(file_bmp, H, W, x0, y0+thickness, x1-1, y0+thickness, thickness, color);
            }
        }

}

void swap_int(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

void draw_line(Rgb **arr, int H, int W, int x0, int y0, int x1, int y1, int thickness, int color[3]) {
    if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0 || thickness <= 0) {
        printf("error 710\n");
        exit(41);
    }
    // вертикальная линия
    if (x0 == x1){
        if (y0 >= y1){
            swap_int(&y0, &y1);
        }
        for (int y = y0; y <= y1; y++){
            for (int j = 0; j < thickness; j++){
                if (H - y >= 0 && x0 - j >= 0 && x0 - j <W && H - y < H){
                    setPixels(arr, x0-j, H-y, color);
                }
            }
            //горизонтальная линия
        }
    }else if (y0 == y1){
            if (x0 >= x1){
                swap_int(&x0, &x1);
            }    
            for(int x=x0; x <= x1; x++){
                for(int j=0; j<thickness; j++){
                    if (H-y0+j >= 0 && x>=0 && x<W && H-y0+j<H){
                        setPixels(arr, x, H-y0+j, color);
                    }
                }
            }
        }
}

Rgb** read_bmp(const char filename[], BitmapFileHeader* bmfh, BitmapInfoHeader* bmif){
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("Error opening input file\n");
        exit(41);
    }
    fread(bmfh, 1, sizeof(BitmapFileHeader), f);
    fread(bmif, 1, sizeof(BitmapInfoHeader), f);
    unsigned int H = bmif->height;
    unsigned int W = bmif->width;

    Rgb **arr = malloc(H * sizeof(Rgb *));
    for (int i = 0; i < H; i++)
    {
        arr[i] = calloc(1, W * sizeof(Rgb) + (4 - (W * sizeof(Rgb)) % 4) % 4);
        fread(arr[i], 1,W * sizeof(Rgb) + (4 - (W * sizeof(Rgb)) % 4) % 4, f);
    }
    fclose(f);
    
    if (bmif->headerSize != 40 || bmif->bitsPerPixel != 24 || bmfh->signature != 0x4D42 || bmif->compression != 0){
        printf("error file bmp");

        exit(41);
    }

    return arr;
}

void write_bmp(const char file_name[], Rgb **arr, int H, int W, BitmapFileHeader* bmfh, BitmapInfoHeader* bmif){
    FILE *ff = fopen(file_name, "wb");
    if (ff == NULL)
    {
        printf("Error opening output file\n");
        exit(42);
    }

    fwrite(bmfh, 1, sizeof(BitmapFileHeader), ff);
    fwrite(bmif, 1, sizeof(BitmapInfoHeader), ff);
    unsigned int w  = W * sizeof(Rgb) + (4 - (W * sizeof(Rgb)) % 4) % 4;
    for (int i = 0; i < H; i++)
    {
        fwrite(arr[i], 1, w, ff);
    }

    fclose(ff);

}

void print_file_header(BitmapFileHeader header){
    printf("signature:\t%x (%hu)\n", header.signature, header.signature);
    printf("filesize:\t%x (%u)\n", header.filesize, header.filesize);
    printf("reserved1:\t%x (%hu)\n", header.reserved1, header.reserved1);
    printf("reserved2:\t%x (%hu)\n", header.reserved2, header.reserved2);
    printf("pixelArrOffset:\t%x (%u)\n", header.pixelArrOffset, header.pixelArrOffset);
}

void print_info_header(BitmapInfoHeader header){
    printf("headerSize:\t%x (%u)\n", header.headerSize, header.headerSize);
    printf("width: \t%x (%u)\n", header.width, header.width);
    printf("height: \t%x (%u)\n", header.height, header.height);
    printf("planes: \t%x (%hu)\n", header.planes, header.planes);
    printf("bitsPerPixel:\t%x (%hu)\n", header.bitsPerPixel, header.bitsPerPixel);
    printf("compression:\t%x (%u)\n", header.compression, header.compression);
    printf("imageSize:\t%x (%u)\n", header.imageSize, header.imageSize);
    printf("xPixelsPerMeter:\t%x (%u)\n", header.xPixelsPerMeter, header.xPixelsPerMeter);
    printf("yPixelsPerMeter:\t%x (%u)\n", header.yPixelsPerMeter, header.yPixelsPerMeter);
    printf("colorsInColorTable:\t%x (%u)\n", header.colorsInColorTable, header.colorsInColorTable);
    printf("importantColorCount:\t%x (%u)\n", header.importantColorCount, header.importantColorCount); 
}

void printHelp(){
    printf("----Information about the use of the course work----\n");
    printf("This program is designed to perform various functions with images of the type.bmp. This program can:\n\n\n");

    printf("--rect -               Draw a rectangle\n");
    printf("These flags are used:\n\n");

    printf("--input, -i             Sets the name of the input image;\n");
    printf("--output, -o            Sets the name of the output image;\n");
    printf("--left_up               Coordinates of the upper-left corner. The value is set in the format `left.up`, where left is the x coordinate, up is the y coordinate;\n");
    printf("--right_down            Coordinates of the lower-right corner. The value is set in the format `right.down`, where right is the x coordinate, down is the y coordinate;\n");
    printf("--thickness             The thickness of the lines. Accepts a number greater than 0 as input;\n");
    printf("--color                 The color of the lines. The color is set by the string `rrr.ggg.bbb`, where rrr/ggg/bbb are the numbers specifying the color component;\n");
    printf("--fill                  The rectangle can be filled in or not. It works as a binary value: there is no flag – false, there is a flag – true;\n");
    printf("--fill_color            Works similarly to the `--color` flag.\n\n\n");
    printf("--ornament -            Make a frame in the form of a pattern: rectangle, circle or semicilcles.\n");
    printf("These flags are used:\n\n");
    printf("--pattern               Pattern. It can take the following values: rectangle and circle, semicircles;\n");
    printf("--color                 The color of the lines. The color is set by the string `rrr.ggg.bbb`, where rrr/ggg/bbb are the numbers specifying the color component;\n");
    printf("--thickness             The thickness of the lines. Accepts a number greater than 0 as input;\n");
    printf("--count                 Amount. Accepts a number greater than 0 as input.\n\n\n");

    printf("--rotate -              Rotate the image (part) by 90/180/270 degrees.\n");
    printf("These flags are used:\n\n");
    printf("--left_up               Coordinates of the upper-left corner. The value is set in the format `left.up`, where left is the x coordinate, up is the y coordinate;\n");
    printf("--right_down            Coordinates of the lower-right corner. The value is set in the format `right.down`, where right is the x coordinate, down is the y coordinate;\n");
    printf("--angle                 The angle of rotation. Possible values: `90`, `180`, `270`.\n");
    printf("\n\nOther functions\n");
    printf("-info -                 Prints information about the image.\n");
    printf("-help, -h -             Displays information about the program and functions.\n");
}
