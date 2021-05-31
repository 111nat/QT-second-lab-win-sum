#include "View.h"

View::View(QWidget*parent): QGLWidget(parent)
{
    visualization_state = VISUALIZATION_QUADS;//VISUALIZATION_QUADSTRIP;
    axisXYZ = axis::x;
    path = "FOURDIX-1.bin";//"Join.bin";
    data.readFile(path);
    //data.readFile("FOURDIX-1.bin");
    //data.readFile("FOURDIX-systolic.bin");
   // data.readFile("p3-after2operation1.bin");
    //data.readFile("torso1.bin");
    
   // setMinimumSize(600, 400);

   
   ui.setupUi(this);
}

void View::initializeGL()
{
    // устанавливаем заполняющий цвет
    qglClearColor(Qt::white);
    // устанавливаем режим сглаживания
    glShadeModel(GL_SMOOTH);
    // задаем модельно-видовую матрицу
    glMatrixMode(GL_MODELVIEW);
    // загрузка единичной матрицы
    glLoadIdentity();

    glGenTextures(1, &VBOtexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void View::resizeGL(int nWidth, int nHeight)
{
    // установка режима матрицы
    glMatrixMode(GL_PROJECTION);
    // загрузка единичной матрицы
    glLoadIdentity();
    // установка ортогонального преобразования
    glOrtho(0.0f, data.getWidth() - 1, 0.0f, data.getHeight() - 1, -1.0f, 1.0f);
    // установка окна обзора
    glViewport(0, 0, nWidth, nHeight);
    update();
}

void View::paintGL()
{
    //qDebug() << "Types:" << QString("String");
    qDebug() << "repaint" << visualization_state;
    // очистка экрана
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (visualization_state)
    {
    case VISUALIZATION_QUADS:
        VisualizationQuads();
        break;
    case VISUALIZATION_QUADSTRIP:
        VisualizationQuadStrip();
        break;
    case VISUALIZATION_TEXTURE:
        VizualizationTexture();
        break;
    }

    qDebug() << "MIN: " << data.getMin();
    qDebug() << "MAX: " << data.getMax();
}

QColor View::TransferFunction(short value)
{
    int c = (value -  data.getMin()) * 255 / (data.getMax() - data.getMin());
    return QColor(c, c, c);
}

void View::VisualizationQuads()
{
    QColor c;
    int w = data.getWidth();
    int h = data.getHeight();
    int d = data.getDepth();
    switch (axisXYZ)
    {
    case View::x:
        for (int y = 0; y < (h - 1); y++)
        {
            for (int x = 0; x < (w - 1); x++)
            {
                glBegin(GL_QUADS);
                c = TransferFunction(data[layer * w * h + y * w + x]);
                qglColor(c);
                glVertex2i(x, y);

                c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
                qglColor(c);
                glVertex2i(x, (y + 1));

                c = TransferFunction(data[layer * w * h + (y + 1) * w + x + 1]);
                qglColor(c);
                glVertex2i((x + 1), (y + 1));

                c = TransferFunction(data[layer * w * h + y * w + x + 1]);
                qglColor(c);
                glVertex2i((x + 1), y);
                glEnd();
            }
        }
        break;
    case View::y:
        for (int y = 0; y < h - 1; y++)
        {
            for (int z = 0; z < d - 1; z++)
            {
                glBegin(GL_QUADS);
                c = TransferFunction(data[w * h * z + y * w + layer]);
                qglColor(c);
                glVertex2i(z, y);

                c = TransferFunction(data[w * h * z + (y + 1) * w + layer]);
                qglColor(c);
                glVertex2i(z, y + 1);

                c = TransferFunction(data[w * h * (z + 1) + (y + 1) * w + layer]);
                qglColor(c);
                glVertex2i(z + 1, y + 1);

                c = TransferFunction(data[w * h * (z + 1) + y * w + layer]);
                qglColor(c);
                glVertex2i(z + 1, y);
                glEnd();
            }
        }
        break;
    case View::z:
        for (int z = 0; z < d - 1; z++)
        {
            for (int x = 0; x < w - 1; x++)
            {
                glBegin(GL_QUADS);
                c = TransferFunction(data[x + z * w * h + layer * w]);
                qglColor(c);
                glVertex2i(x, z);

                c = TransferFunction(data[x + (z + 1) * w * h + layer * w]);
                qglColor(c);
                glVertex2i(x, z + 1);

                c = TransferFunction(data[(x + 1) + (z + 1) * w * h + layer * w]);
                qglColor(c);
                glVertex2i(x + 1, z + 1);

                c = TransferFunction(data[(x + 1) + z * w * h + layer * w]);
                qglColor(c);
                glVertex2i(x + 1, z);
                glEnd();
            }
        }
        break;
    }

    

}

void View::keyPressEvent(QKeyEvent* event)
{
    if (event->nativeVirtualKey() == Qt::Key_U)
    {
        // Подняться на слой выше
        
        if ( layer < data.getDepth()-1)
        {
            layer++;
        }
       
    }
    else if (event->nativeVirtualKey() == Qt::Key_D)
    {
        // Опуститься на слой ниже
        if (layer != 0)
        {
            layer--;
        }
    }
    else if (event->nativeVirtualKey() == Qt::Key_N)
    {
        qDebug() << QString("String");
       
        // Переключиться на следующий тип рендеринга
        if (visualization_state == VISUALIZATION_QUADS)
        {
            visualization_state = VISUALIZATION_QUADSTRIP;
        }
        else if (visualization_state == VISUALIZATION_QUADSTRIP)
        {
            visualization_state = VISUALIZATION_TEXTURE;
        }
        else
        {
            visualization_state = VISUALIZATION_QUADS;
        }
        layer = 0;
    }
    else if (event->nativeVirtualKey() == Qt::Key_W)
    {
        if (axisXYZ == axis::x)
        {
            axisXYZ = axis::y;
            resizeGL(600, 400);
            
            //resizeGL(data.getHeight() * 10, data.getWidth() * 10);
        }
        else if (axisXYZ == axis::y)
        {
            axisXYZ = axis::z;
            resizeGL(600, 400);
            //resizeGL(data.getWidth() * 10, data.getHeight() * 10);
        }
        else if (axisXYZ == axis::z)
        {
            axisXYZ = axis::x;
            
            resizeGL(600, 400);
        }

        layer = 0;
    }
    else if (event->nativeVirtualKey() == Qt::Key_B)
    {
        if (path == "Join.bin")
        {
            path = "FOURDIX-1.bin";
            data.clear_arr();
            data.readFile(path);
        }
        else if (path == "FOURDIX-1.bin")
        {
            path = "FOURDIX-systolic.bin";
            data.clear_arr();
            data.readFile(path);
        }
        else if (path == "FOURDIX-systolic.bin")
        {
            path = "p3-after2operation1.bin";
            data.clear_arr();
            data.readFile(path);
        }
        else if (path == "p3-after2operation1.bin")
        {
            path = "torso1.bin";
            data.clear_arr();
            data.readFile(path);
        }
        else if (path == "torso1.bin")
        {
            path = "Join.bin";
            data.clear_arr();
            data.readFile(path);
        }


        resizeGL(data.getWidth() * 10, data.getHeight() * 10);
        //resizeGL(600, 400);

    }
    else if (event->nativeVirtualKey() == Qt::Key_8)
    {
        data.data_max_plus();
    }
    else if (event->nativeVirtualKey() == Qt::Key_2)
    {
        data.data_max_minus();
    }
    else if (event->nativeVirtualKey() == Qt::Key_6)
    {
        data.data_min_plus();
    }
    else if (event->nativeVirtualKey() == Qt::Key_4)
    {
        data.data_min_minus();
    }
    else if (event->nativeVirtualKey() == Qt::Key_M)
    {
        int newMax = QInputDialog::getInt(0, "Set max", "Max is:", 1, 1);
        if (newMax > 4000)
        {
            newMax = 4000;
        }
        
        if (newMax < data.getMin())
        {
            newMax = data.getMin();
        }
        data.set_max(newMax);
    }
    else if (event->nativeVirtualKey() == Qt::Key_L)
    {
        int newMin = QInputDialog::getInt(0, "Set min", "Min is:", 1, -4000);
        if (newMin < -4000)
        {
            newMin = -4000;
        }

        if (newMin > data.getMax())
        {
            newMin = data.getMax();
        }
        data.set_min(newMin);
    }
    update();
}

void View::VisualizationQuadStrip()    //QuadStrip, когда первый четырехугольник рисуется 4 вершинами, а последующие - 2 вершинами, присоединенными к предыдущему четырехугольнику
{
    QColor c;
    int w = data.getWidth();
    int h = data.getHeight();
    int d = data.getDepth();

    switch (axisXYZ)
    {
    case x:
        for (int y = 0; y < h - 1; y++)
        {
            glBegin(GL_QUAD_STRIP);
            for (int x = 0; x < w; x++)
            {
                c = TransferFunction(data[layer * w * h + y * w + x]);
                //glColor3f(c, c, c);
                qglColor(c);
                glVertex2i(x, y);

                c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
                //glColor3f(c, c, c);
                qglColor(c);
                glVertex2i(x, y + 1);
            }
            glEnd();
        }
        break;

    case y:
        for (int y = 0; y < h - 1; y++)
        {
            glBegin(GL_QUAD_STRIP);
            for (int z = 0; z < d; z++)
            {
                c = TransferFunction(data[w * h * z + y * w + layer]);
                //glColor3f(c, c, c);
                qglColor(c);
                glVertex2i(z, y);

                c = TransferFunction(data[w * h * z + (y + 1) * w + layer]);
                //glColor3f(c, c, c);
                qglColor(c);
                glVertex2i(z, y + 1);
            }
            glEnd();
        }
        break;

    case z:
        for (int z = 0; z < d - 1; z++)
        {
            glBegin(GL_QUAD_STRIP);
            for (int x = 0; x < w; x++)
            {
                c = TransferFunction(data[x + z * w * h + layer * w]);
                //glColor3f(c, c, c);
                qglColor(c);
                glVertex2i(x, z);

                c = TransferFunction(data[x + (z + 1) * w * h + layer * w]);
                //glColor3f(c, c, c);
                qglColor(c);
                glVertex2i(x, z + 1);
            }
            glEnd();
        }
        break;
    }
}

void View::VizualizationTexture()  //рисует один прямоугольник с наложенной текстурой
{
    genTextureImage();
    Load2dTexture();

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2i(0, 0);
    glTexCoord2f(0, 1);
    glVertex2i(0, data.getHeight());
    glTexCoord2f(1, 1);
    glVertex2i(data.getWidth(), data.getHeight());
    glTexCoord2f(1, 0);
    glVertex2i(data.getWidth(), 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void View::genTextureImage()   //генерировать изображение из томограммы при помощи созданной Transfer Function
{
    QColor c;
    int w = data.getWidth();
    int h = data.getHeight();
    int d = data.getDepth();
    textureImage = QImage(w, h, QImage::Format_RGB32);
    qDebug() << "GEN_TEXTURE";

    switch (axisXYZ)
    {
    case x:
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
            {
                c = TransferFunction(data[layer * w * h + w * y + x]); //* 255;
                /*if (c > 255)
                    c = 255;
                else if (c < 0)
                    c = 0;*/
                textureImage.setPixelColor(x, y, c);//QColor(c, c, c));
            }
        break;

    case y:
        for (int y = 0; y < h; y++)
            for (int z = 0; z < d; z++)
            {
                c = TransferFunction(data[w * h * z + y * w + layer]); //* 255;
                /*if (c > 255)
                    c = 255;
                else if (c < 0)
                    c = 0;*/
                textureImage.setPixelColor(z, y, c);//QColor(c, c, c));
            }
        break;
    case z:
        for (int z = 0; z < d; z++)
            for (int x = 0; x < w; x++)
            {
                c = TransferFunction(data[x + z * w * h + layer * w]);// *255;
                /*if (c > 255)
                    c = 255;
                else if (c < 0)
                    c = 0;*/
                textureImage.setPixelColor(x, z, c);// QColor(c, c, c));
            }
        break;
    }
}

void View::Load2dTexture() //Функции прямого доступа к состоянию
{
    qDebug() << "LOAD_TEXTURE";
    glBindTexture(GL_TEXTURE_2D, VBOtexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(),
        0, GL_BGRA, GL_UNSIGNED_BYTE, textureImage.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}










