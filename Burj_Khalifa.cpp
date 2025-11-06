// skyline_cityscape_ultra_dense.cpp  (C++11)
// Ultra-dense cityscape that fills the entire horizon with buildings

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

struct Canvas
{
    int H, W;
    vector<string> g;
    Canvas(int h,int w,char fill=' '):H(h),W(w),g(h,string(w,fill)) {}
    inline void set(int r,int c,char ch)
    {
        if(r>=0&&r<H&&c>=0&&c<W) g[r][c]=ch;
    }
    inline void hline(int r,int c1,int c2,char ch='-')
    {
        if(c1>c2) std::swap(c1,c2);
        for(int c=c1; c<=c2; c++) set(r,c,ch);
    }
    inline void vline(int r1,int r2,int c,char ch='|')
    {
        if(r1>r2) std::swap(r1,r2);
        for(int r=r1; r<=r2; r++) set(r,c,ch);
    }
    void rect(int top,int left,int h,int w,char fill='#',bool windows=false,bool ribs=false)
    {
        for(int i=0; i<h; i++)
        {
            for(int j=0; j<w; j++)
            {
                char ch=fill;
                if (windows && i%2==0 && j%3==1) ch='.';
                if (ribs && j%6==0) ch='|';
                set(top+i,left+j,ch);
            }
        }
        for(int j=0; j<w; j++)
        {
            set(top,left+j,'=');
            set(top+h-1,left+j,'=');
        }
        for(int i=0; i<h; i++)
        {
            set(top+i,left,'|');
            set(top+i,left+w-1,'|');
        }
    }
    void print()const
    {
        for(size_t i=0; i<g.size(); ++i) cout<<g[i]<<"\n";
    }
};

static inline int left_from_center(int centerCol, int w)
{
    return centerCol - (w - 1) / 2;
}

void add_building(Canvas& cv, int baseRow, int center, int shaftW, int shaftH,
                  int stepW = 0, int stepH = 0, bool crown=true, bool antenna=false)
{
    int minC = 2 + shaftW/2;
    int maxC = cv.W - 3 - shaftW/2;
    if (center < minC) center = minC;
    if (center > maxC) center = maxC;

    int left = center - shaftW/2;
    int top  = baseRow - shaftH;
    cv.rect(top, left, shaftH, shaftW, '#', true, (shaftW>=14));

    if (stepW>0 && stepH>0)
    {
        int stepLeft = left + (shaftW - stepW)/2;
        int seamRow = top - 1;
        cv.hline(seamRow, left, left+shaftW-1, '-');
        int stepTop = seamRow - stepH;
        cv.rect(stepTop, stepLeft, stepH, stepW, '#', true);
        if (crown && stepW>6)
        {
            int cW = stepW - 4, cH = std::max(2, stepH/2);
            int cLeft = stepLeft + (stepW - cW)/2;
            int cTop = stepTop - cH;
            cv.rect(cTop, cLeft, cH, cW, '#', true);
            if (antenna)
            {
                int aBase = cTop - 1;
                int a1 = cLeft + cW/3, a2 = cLeft + 2*cW/3;
                cv.vline(aBase-3, aBase, a1, '|');
                cv.set(aBase-4, a1, '^');
                cv.vline(aBase-5, aBase, a2, '|');
                cv.set(aBase-6, a2, '^');
            }
        }
    }
    else if (crown)
    {
        int cW = std::max(6, shaftW - 6), cH = std::max(2, shaftH/8);
        int cLeft = left + (shaftW - cW)/2;
        int cTop = top - cH - 1;
        cv.hline(top-1, left, left+shaftW-1, '-');
        cv.rect(cTop, cLeft, cH, cW, '#', true);
        if (antenna)
        {
            int aBase = cTop - 1;
            int a = cLeft + cW/2;
            cv.vline(aBase-4, aBase, a, '|');
            cv.set(aBase-5, a, '^');
        }
    }
}

inline void build_tiny_box(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 6, 8, 0,0, true,false); }
inline void build_small_ribbed(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 10, 14, 0,0, true,false); }
inline void build_medium_crown(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 12, 18, 0,0, true,false); }
inline void build_medium_step_antenna(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 12, 16, 8,4, true,true); }
inline void build_wide_block(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 24, 10, 0,0, true,false); }
inline void build_wide_step(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 26, 12, 16,3, true,false); }
inline void build_tall_slim(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 6, 32, 0,0, true,false); }
inline void build_tall_ribbed(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 14, 28, 0,0, true,false); }
inline void build_double_step(Canvas& cv, int baseRow, int c) {
    add_building(cv, baseRow, c, 18, 18, 12,4, true,false);
    add_building(cv, baseRow-22, c, 12, 10, 8,3, true,false);
}
inline void build_ziggurat3(Canvas& cv, int baseRow, int c) {
    add_building(cv, baseRow, c, 20, 12, 0,0, true,false);
    add_building(cv, baseRow-13, c, 14, 9, 0,0, true,false);
    add_building(cv, baseRow-23, c, 8, 7, 0,0, true,true);
}
inline void build_ziggurat4(Canvas& cv, int baseRow, int c) {
    add_building(cv, baseRow, c, 28, 10, 0,0, true,false);
    add_building(cv, baseRow-11, c, 22, 8, 0,0, true,false);
    add_building(cv, baseRow-20, c, 16, 7, 0,0, true,false);
    add_building(cv, baseRow-28, c, 10, 6, 0,0, true,true);
}
inline void build_crown_only(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 12, 14, 0,0, true,false); }
inline void build_antenna_only(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 8, 16, 0,0, false,true); }
inline void build_pyramid(Canvas& cv, int baseRow, int c) {
    add_building(cv, baseRow, c, 16, 10, 0,0, true,false);
    add_building(cv, baseRow-11, c, 12, 8, 0,0, true,false);
    add_building(cv, baseRow-20, c, 8, 6, 0,0, true,true);
}
inline void build_L_left(Canvas& cv, int baseRow, int c) {
    add_building(cv, baseRow, c, 14, 18, 0,0, true,false);
    add_building(cv, baseRow, c-7, 8, 10, 0,0, true,false);
}
inline void build_L_right(Canvas& cv, int baseRow, int c) {
    add_building(cv, baseRow, c, 14, 18, 0,0, true,false);
    add_building(cv, baseRow, c+7, 8, 10, 0,0, true,false);
}
inline void build_needle(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 6, 26, 0,0, true,true); }
inline void build_campus(Canvas& cv, int baseRow, int c) {
    add_building(cv, baseRow, c-10, 12, 8, 0,0, true,false);
    add_building(cv, baseRow, c, 16, 8, 0,0, true,false);
    add_building(cv, baseRow, c+10, 12, 8, 0,0, true,false);
}
inline void build_glass_tower(Canvas& cv, int baseRow, int c) {
    add_building(cv, baseRow, c, 10, 22, 0,0, true,false);
    for (int i = 2; i < 20; i += 3) {
        cv.hline(baseRow - i, c - 4, c + 4, '.');
    }
}
inline void build_industrial(Canvas& cv, int baseRow, int c) {
    add_building(cv, baseRow, c, 20, 8, 0,0, false,false);
    add_building(cv, baseRow-9, c-5, 8, 6, 0,0, false,false);
    add_building(cv, baseRow-9, c+5, 8, 6, 0,0, false,false);
    cv.vline(baseRow-18, baseRow-9, c-2, '|');
    cv.vline(baseRow-18, baseRow-9, c+2, '|');
    cv.set(baseRow-19, c-2, 'o');
    cv.set(baseRow-19, c+2, 'o');
}
inline void build_low_rise(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 16, 6, 0,0, false,false); }
inline void build_mid_rise(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 14, 12, 0,0, true,false); }
inline void build_high_rise(Canvas& cv, int baseRow, int c) { add_building(cv, baseRow, c, 12, 20, 0,0, true,true); }

int main()
{
    const int H=100, W=200;
    Canvas cv(H,W,' ');
    const int baseRow = H-4;
    const int centerCol = W/2;

 
    cv.hline(H-3,0,W-1,'_');

  
    int widths[]  = {64,54,46,38,30,24,20,16,12,9,6,4};
    int heights[] = {10,10, 9, 9, 8, 7, 6, 6, 5, 4,4,4};
    const int nTiers = sizeof(widths)/sizeof(widths[0]);
    int rowTop=baseRow;

    for(int i=0; i<nTiers; i++)
    {
        int w=widths[i], h=heights[i];
        int left=left_from_center(centerCol,w);
        rowTop-=h;
        cv.rect(rowTop,left,h,w,'#',true);
        if(i+1<nTiers)
        {
            rowTop--;
            int mw=max(w,widths[i+1]);
            int seamL=left_from_center(centerCol,mw), seamR=seamL+mw-1;
            cv.hline(rowTop,seamL,seamR,'-');
        }
    }
    int spireLen=22;
    for(int r=rowTop-spireLen; r<=rowTop-1; r++) cv.vline(r,r,centerCol,'|');
    cv.set(rowTop-spireLen-1,centerCol,'^');

 
    int podiumH=6, podiumW=80, podiumLeft=left_from_center(centerCol,podiumW);
    cv.rect(baseRow-podiumH,podiumLeft,podiumH,podiumW,'#',true);
    cv.rect(baseRow-8,podiumLeft-12,8,14,'#',true);
    cv.rect(baseRow-9,podiumLeft+podiumW-2,9,14,'#',true);

    
    void (*building_functions[])(Canvas&, int, int) = {
        build_tiny_box, build_small_ribbed, build_medium_crown, build_medium_step_antenna,
        build_wide_block, build_wide_step, build_tall_slim, build_tall_ribbed,
        build_double_step, build_ziggurat3, build_ziggurat4, build_crown_only,
        build_antenna_only, build_pyramid, build_L_left, build_L_right,
        build_needle, build_campus, build_glass_tower, build_industrial,
        build_low_rise, build_mid_rise, build_high_rise
    };

    const int num_building_types = sizeof(building_functions) / sizeof(building_functions[0]);


    int current_pos = 5;

 
    while(current_pos < 90) {
        int building_type = rand() % num_building_types;
        building_functions[building_type](cv, baseRow, current_pos);

        
        int spacing = 8 + (rand() % 12); 
        current_pos += spacing;

        
        if(current_pos > 85) break;
    }

   
    current_pos = 115;
    while(current_pos < 195) {
        int building_type = rand() % num_building_types;
        building_functions[building_type](cv, baseRow, current_pos);

        int spacing = 8 + (rand() % 12);
        current_pos += spacing;
    }

 
    build_low_rise(cv, baseRow, 2);
    build_small_ribbed(cv, baseRow, 8);
    build_medium_crown(cv, baseRow, 15);
    build_tall_slim(cv, baseRow, 22);
    build_wide_block(cv, baseRow, 30);
    build_industrial(cv, baseRow, 40);
    build_pyramid(cv, baseRow, 50);
    build_ziggurat3(cv, baseRow, 60);
    build_double_step(cv, baseRow, 70);
    build_campus(cv, baseRow, 80);

    
    build_glass_tower(cv, baseRow, 92);
    build_high_rise(cv, baseRow, 98);
    build_mid_rise(cv, baseRow, 104);

  
    build_mid_rise(cv, baseRow, 108);
    build_high_rise(cv, baseRow, 112);
    build_glass_tower(cv, baseRow, 118);

   
    build_campus(cv, baseRow, 125);
    build_double_step(cv, baseRow, 135);
    build_ziggurat3(cv, baseRow, 145);
    build_pyramid(cv, baseRow, 155);
    build_industrial(cv, baseRow, 165);
    build_wide_block(cv, baseRow, 175);
    build_tall_slim(cv, baseRow, 185);
    build_medium_crown(cv, baseRow, 192);
    build_small_ribbed(cv, baseRow, 198);
    build_low_rise(cv, baseRow, 198);

    
    int front_base = baseRow - 2;
    current_pos = 10;
    while(current_pos < 190) {
        if(current_pos < 80 || current_pos > 120) { 
            int height_variation = rand() % 3;
            if(height_variation == 0) build_low_rise(cv, front_base, current_pos);
            else if(height_variation == 1) build_small_ribbed(cv, front_base, current_pos);
            else build_medium_crown(cv, front_base, current_pos);
        }
        current_pos += 12 + (rand() % 15);
    }

    int very_front_base = baseRow - 1;
    current_pos = 15;
    while(current_pos < 185) {
        if(current_pos < 75 || current_pos > 125) { 
            build_low_rise(cv, very_front_base, current_pos);
        }
        current_pos += 8 + (rand() % 10);
    }

    cv.print();
    return 0;
}

