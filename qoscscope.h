#ifndef QOSCSCOPE_H
#define QOSCSCOPE_H

#include <QtCore>
#include <QWidget>
#include <QPixmap>

class QOscScope : public QWidget
{
    Q_OBJECT

public:
    QOscScope(qint32 s_width=960,qint32 s_height = 256,QWidget *parent = 0);
    ~QOscScope();

    qint64 smapleFrequency();
    void setSampleFrequenvy(const qint64 fs);
public slots:
    void setData(const qint32 &data);
    void setData(const qint32 *src, qint32 size);
    void setData(const qreal &data);

protected:
    void paintEvent(QPaintEvent *e);
    void timerEvent(QTimerEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);
private:
    QBasicTimer *refreshTimer;      //refresh screen

    QPixmap *pixFrame;  //use Pixmap as framebuffer
    QPixmap *pixGrid;

    bool gridOn;
    bool ctrPressed;
    bool mousePressed;
    QPoint lastPos;

    qint16 refreshRate;
    qint64 m_fs;
    qint32 m_width;
    qint32 m_height;
    qint32 m_showWidth;
    qint32 m_showHeight;
    qint32 m_wFoucs;
    qint32 m_hFoucs;
    qreal aspect;
    void initGrid();
    void initPixFrame();
    void clearPixFrame();
};

#endif // QOSCSCOPE_H
