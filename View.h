#pragma once

//#include <QtWidgets/QWidget>
#include "ui_View.h"

#include <QDialog>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QInputDialog>

#include <QGLWidget>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <vector>
#include <QKeyEvent>
//#include <QOpenGLWidget>

class DemoDialog;


class Data
{
    int width;
    int height;
    int depth;

    float x;
    float y;
    float z;

    std::vector<short> arr;

    short min;
    short max;
public:
    short getMin() 
    { 
        return min;
    };

    short getMax() 
    { 
        return max;
    };

    int getWidth() { return width; };
    int getHeight() { return height; };
    int getDepth() { return depth; };
    std::string readFile(std::string filename)
    {
        std::ifstream file(filename, std::ios::binary|std::ios::in);
       
        
        //file.open(filename, std::fstream::in);
        if (!file.is_open())
        {
            std::cout << "Ошибка открытия файла" << std::endl;
            return std::string();
        }
        else
        {
            int textHelper1 = int();
            std::cout << "Файл открыт для чтения" << std::endl;

            file.read((char*)&textHelper1, sizeof textHelper1);
            width = textHelper1;

            file.read((char*)&textHelper1, sizeof textHelper1);
            height = textHelper1;

            file.read((char*)&textHelper1, sizeof textHelper1);
            depth = textHelper1;


            float textHelper2 = float();

            file.read((char*)&textHelper2, sizeof textHelper2);
            x = textHelper2;

            file.read((char*)&textHelper2, sizeof textHelper2);
            y = textHelper2;

            file.read((char*)&textHelper2, sizeof textHelper2);
            z = textHelper2;

            short textHelper3 = short();
            while (file.read((char*)&textHelper3, sizeof textHelper3))
            {
                if (arr.size() == 0)
                {
                    min = textHelper3;
                    max = textHelper3;
                }
                else
                {
                    if (min > textHelper3)
                    {
                        min = textHelper3;
                    }

                    if (max < textHelper3)
                    {
                        max = textHelper3;
                    }
                }
                arr.push_back(textHelper3);
            }
            
            file.close();
        }
        

        return std::string();
        
    }
    
    short& operator[](int index)
    {
        if (index < arr.size() && index > -1)
        {
            return arr[index];
        }
        
        if (index >= arr.size())
        {
            return arr[arr.size() - 1];
        }

        if (index < 0)
        {
            return arr[0];
        }
    }

    void clear_arr()
    {
        arr.clear();
    }

    void data_max_plus()
    {
        max += 500;
    }

    void data_max_minus()
    {
        max -= 500;
    }

    void data_min_plus()
    {
        min += 500;
    }

    void data_min_minus()
    {
        min -= 500;
    }

    void set_max(int newMax)
    {
        this->max = newMax;
    }
    void set_min(int newMin)
    {
        this->min = newMin;
    }
};

class View : public QGLWidget
{
    Q_OBJECT
        

public:
    View(QWidget*parent = Q_NULLPTR);
    
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();

    void VisualizationQuads();
    void VizualizationTexture();
    void VisualizationQuadStrip();

    QColor TransferFunction(short value);

    void keyPressEvent(QKeyEvent* event);

    void genTextureImage();
    void Load2dTexture();
private:
    Ui::ViewClass ui;

    enum vis_state
    {
        VISUALIZATION_QUADS,
        VISUALIZATION_QUADSTRIP,
        VISUALIZATION_TEXTURE
    };
    vis_state visualization_state;

    Data data;

    int layer = 0;

    enum axis
    {
        x,
        y,
        z
    };

    axis axisXYZ;

    QImage textureImage;

    GLuint VBOtexture;

    std::string path;
    
    
    
    /*QSpinBox* minimumSpinBox;
    QSpinBox* maximumSpinBox;

    QLabel* minimumLabel;
    QLabel* maximumLabel;*/
     

};


