#ifndef BLOCK_TIMER_H
#define BLOCK_TIMER_H


#include <qdatetime.h>
#include <qtimer.h>

class BlockTimer : public QObject
{
    Q_OBJECT
  private :
    QTimer *block_timer;
    int autoblock_time;
  public :
    QTime action_time;
    bool block_flag;

    BlockTimer(int abt);
    ~BlockTimer();
    
    void Start();
    void Stop();
  private slots :
    void CheckForBlock();
};
	
#endif
