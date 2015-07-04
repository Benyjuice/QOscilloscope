#include "qoscscope.h"
#include <QResizeEvent>
#include <QPainter>
#include <QDebug>
#include <QtMath>

QOscScope::QOscScope(qint32 s_width, qint32 s_height, QWidget *parent)
    : QWidget(parent),
      m_height(s_height),
      m_width(s_width),
      m_showHeight(s_height),
      m_showWidth(s_width/2),
      m_fs(10000),
      ctrPressed(false  ),gridOn(true),
      pixFrame(Q_NULLPTR),
      pixGrid(Q_NULLPTR),
      refreshTimer(Q_NULLPTR),
      refreshRate(50),
      aspect ((qreal)m_height/m_width)
{
//    setMaximumHeight(500);
    setMaximumWidth(m_fs*10);
    resize(s_width,s_height);
    initPixFrame();

    refreshTimer = new QBasicTimer;
    refreshTimer->start(1000/refreshRate,this);  //freashRate
}

QOscScope::~QOscScope()
{

}

void QOscScope::setData(const qint32 &data)
{

}

void QOscScope::setData(const qint32 *src, qint32 size)
{

}

void QOscScope::setData(const qreal &data)
{

}

void QOscScope::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.drawPixmap(0,0,*pixGrid);
    painter.drawPixmap(0,0,m_width,m_height,pixFrame->
                       copy(m_wFoucs-m_showWidth/2,m_hFoucs-m_showHeight/2,
                            m_showWidth,m_showHeight).scaled(m_width,m_height));
}

void QOscScope::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == refreshTimer->timerId())
        update();
}

void QOscScope::wheelEvent(QWheelEvent *e)
{
    int diff=e->delta();
    if (ctrPressed) {
        m_showWidth -= diff;
        if (m_showWidth > pixFrame->width())
            m_showWidth = pixFrame->width();
        if (m_showWidth < 10)
            m_showWidth = 10;
    } else {
        m_showHeight -= diff;
        if (m_showHeight > pixFrame->height())
            m_showHeight = pixFrame->height();
        if (m_showHeight < 10)
            m_showHeight = 10;
    }
}

void QOscScope::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_D:
        m_wFoucs += 20;
        if (m_wFoucs > pixFrame->width()-m_showWidth/2)
            m_wFoucs =pixFrame->width()-m_showWidth/2;
        break;

    case Qt::Key_A:
        m_wFoucs -= 20;
        if (m_wFoucs < m_showWidth/2)
            m_wFoucs = m_showWidth/2;
        break;

    case Qt::Key_S:
        m_hFoucs -= 20;
        if (m_hFoucs < m_showHeight/2)
            m_hFoucs = m_showHeight/2;
        break;

    case Qt::Key_W:
        m_hFoucs +=20;
        if (m_hFoucs > pixFrame->height()-m_showHeight/2)
            m_hFoucs = pixFrame->height()-m_showHeight/2;

    case Qt::Key_Control:
        ctrPressed = true;
    default:
        break;
    }

}

void QOscScope::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Control:
        ctrPressed = false;
        break;
    default:
        break;
    }

}

void QOscScope::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        lastPos = e->globalPos();
    }

}

void QOscScope::mouseReleaseEvent(QMouseEvent *e)
{
    mousePressed = false;

}

void QOscScope::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed) {
        QPoint currentPos = e->globalPos();
        m_wFoucs -= (currentPos-lastPos).x()*m_showWidth/m_width;
        m_hFoucs -= (currentPos-lastPos).y()*m_showHeight/m_height;
        if (m_hFoucs > pixFrame->height()-m_showHeight/2)
            m_hFoucs = pixFrame->height()-m_showHeight/2;
        if (m_hFoucs < m_showHeight/2)
            m_hFoucs = m_showHeight/2;
        if (m_wFoucs > pixFrame->width()-m_showWidth/2)
                   m_wFoucs =pixFrame->width()-m_showWidth/2;
        if (m_wFoucs < m_showWidth/2)
                    m_wFoucs = m_showWidth/2;

        lastPos = currentPos;
    }

}

void QOscScope::resizeEvent(QResizeEvent *e)
{
    m_height = e->size().height();
    m_width = e->size().width();
    aspect = (qreal)m_height/m_width;
    initGrid();
}

void QOscScope::initGrid()
{
    if (pixGrid) {
        delete pixGrid;
        pixGrid = Q_NULLPTR;
    }
    pixGrid = new QPixmap(m_width,m_height);
    pixGrid->fill(Qt::black);
    QPainter painter(pixGrid);
    QPen pen(Qt::green);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

    if (gridOn) {
        for (int i=0;i<10;++i) {
            painter.drawLine(i*m_width/10,0,i*m_width/10,m_height);
            painter.drawLine(0,i*m_height/10,m_width,i*m_height/10);
        }
    }
}

void QOscScope::initPixFrame()
{
    if (pixFrame) {
        delete pixFrame;
        pixFrame = Q_NULLPTR;
    }
    pixFrame = new QPixmap(m_width*2,aspect*m_width*2);
    m_wFoucs = pixFrame->width()/2;
    m_hFoucs = pixFrame->height()/2;
    clearPixFrame();
}

void QOscScope::clearPixFrame()
{
    QColor color=Qt::white;
    color.setAlpha(0);
    if (pixFrame)
        pixFrame->fill(color);

    QPen pen;
//    pen.setWidth(2);
    pen.setColor(Qt::red);
    QPainter p(pixFrame);
    p.setPen(pen);
    QPainterPath path;
    path.moveTo(0,pixFrame->height()/2);
    for (int i=1;i<pixFrame->width();i++) {
        path.lineTo(i,pixFrame->height()/2 *(1+qSin(qRadiansToDegrees(2*M_PI*0.001*i))));
    }
    p.drawPath(path);
    p.drawLine(0,0,pixFrame->width(),pixFrame->height());
}
