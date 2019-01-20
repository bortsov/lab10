/*
 * PinInterface.h
 *
 *  Created on: 01 сент. 2015 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_PININTERFACE_H_
#define INCLUDE_PININTERFACE_H_


class PinInterface {
public:
    virtual void setDirectionOutput() = 0;
    virtual void setDirectionInput() = 0;
    virtual bool isDirectionOutput() const = 0;
    virtual bool getState() const = 0;
    virtual void on() = 0;
    virtual void off() = 0;
    virtual bool isHwInitialized() const {return true;}
    virtual void initHw() {}
    virtual ~PinInterface(){}
};


#endif /* INCLUDE_PININTERFACE_H_ */
