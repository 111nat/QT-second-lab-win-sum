#include "View.h"

View::View(QWidget*parent): QGLWidget(parent)
{
    visualization_state = VISUALIZATION_QUADS;
    //data.readFile("Join.bin");
    //data.readFile("FOURDIX-1.bin");
    //data.readFile("FOURDIX-systolic.bin");
   // data.readFile("p3-after2operation1.bin");
    data.readFile("torso1.bin");
    ui.setupUi(this);
}

void View::initializeGL()
{
    // ������������� ����������� ����
    qglClearColor(Qt::white);
    // ������������� ����� �����������
    glShadeModel(GL_SMOOTH);
    // ������ ��������-������� �������
    glMatrixMode(GL_MODELVIEW);
    // �������� ��������� �������
    glLoadIdentity();
}

void View::resizeGL(int nWidth, int nHeight)
{
    // ��������� ������ �������
    glMatrixMode(GL_PROJECTION);
    // �������� ��������� �������
    glLoadIdentity();
    // ��������� �������������� ��������������
    glOrtho(0.0f, data.getWidth() - 1, 0.0f, data.getHeight() - 1, -1.0f, 1.0f);
    // ��������� ���� ������
    glViewport(0, 0, nWidth, nHeight);
    update();
}

void View::paintGL()
{
    //qDebug() << "Types:" << QString("String");
    qDebug() << "repaint" << visualization_state;
    // ������� ������
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

}

void View::keyPressEvent(QKeyEvent* event)
{
    if (event->nativeVirtualKey() == Qt::Key_U)
    {
        // ��������� �� ���� ����
        if ( layer < data.getDepth() )
        {
            layer++;
        }

    }
    else if (event->nativeVirtualKey() == Qt::Key_D)
    {
        // ���������� �� ���� ����
        if (layer != 0)
        {
            layer--;
        }
    }
    else if (event->nativeVirtualKey() == Qt::Key_N)
    {
        // ������������� �� ��������� ��� ����������
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
    }
    // ��������� �� ���� ����
    update();
}