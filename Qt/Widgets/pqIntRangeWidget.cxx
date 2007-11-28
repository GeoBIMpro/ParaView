/*=========================================================================

   Program:   ParaView
   Module:    pqIntRangeWidget.cxx

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.1. 

   See License_v1.1.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "pqIntRangeWidget.h"

// Qt includes
#include <QLineEdit>
#include <QSlider>
#include <QHBoxLayout>
#include <QIntValidator>

pqIntRangeWidget::pqIntRangeWidget(QWidget* p)
  : QWidget(p) 
{
  this->BlockUpdate = false;
  this->Value = 0;
  this->Minimum = 0;
  this->Maximum = 1;
  this->StrictRange = false;

  QHBoxLayout* l = new QHBoxLayout(this);
  l->setMargin(0);
  this->Slider = new QSlider(Qt::Horizontal, this);
  this->Slider->setRange(0,1);
  l->addWidget(this->Slider);
  this->Slider->setObjectName("Slider");
  this->LineEdit = new QLineEdit(this);
  l->addWidget(this->LineEdit);
  this->LineEdit->setObjectName("LineEdit");
  this->LineEdit->setValidator(new QIntValidator(this->LineEdit));
  this->LineEdit->setText(QString().setNum(this->Value));

  QObject::connect(this->Slider, SIGNAL(valueChanged(int)),
                   this, SLOT(sliderChanged(int)));
  QObject::connect(this->LineEdit, SIGNAL(textChanged(const QString&)),
                   this, SLOT(textChanged(const QString&)));
  
}

//-----------------------------------------------------------------------------
pqIntRangeWidget::~pqIntRangeWidget()
{
}

//-----------------------------------------------------------------------------
int pqIntRangeWidget::value() const
{
  return this->Value;
}

//-----------------------------------------------------------------------------
void pqIntRangeWidget::setValue(int val)
{
  if(this->Value == val)
    {
    return;
    }

  if(!this->BlockUpdate)
    {
    // set the slider 
    this->Slider->blockSignals(true);
    this->Slider->setValue(val);
    this->Slider->blockSignals(false);

    // set the text
    this->LineEdit->blockSignals(true);
    this->LineEdit->setText(QString().setNum(val));
    this->LineEdit->blockSignals(false);
    }

  this->Value = val;
  emit this->valueChanged(this->Value);
}

//-----------------------------------------------------------------------------
int pqIntRangeWidget::maximum() const
{
  return this->Maximum;
}

//-----------------------------------------------------------------------------
void pqIntRangeWidget::setMaximum(int val)
{
  this->Maximum = val;
  this->Slider->setMaximum(val);
  this->updateValidator();
}

//-----------------------------------------------------------------------------
int pqIntRangeWidget::minimum() const
{
  return this->Minimum;
}

//-----------------------------------------------------------------------------
void pqIntRangeWidget::setMinimum(int val)
{
  this->Minimum = val;
  this->Slider->setMinimum(val);
  this->updateValidator();
}

//-----------------------------------------------------------------------------
void pqIntRangeWidget::updateValidator()
{
  if(this->StrictRange)
    {
    this->LineEdit->setValidator(new QIntValidator(this->minimum(),
        this->maximum(), this->LineEdit));
    }
  else
    {
    this->LineEdit->setValidator(new QIntValidator(this->LineEdit));
    }
}

//-----------------------------------------------------------------------------
bool pqIntRangeWidget::strictRange() const
{
  const QIntValidator* dv =
    qobject_cast<const QIntValidator*>(this->LineEdit->validator());
  return dv->bottom() == this->minimum() && dv->top() == this->maximum();
}
  
void pqIntRangeWidget::setStrictRange(bool s)
{
  this->StrictRange = s;
  this->updateValidator();
}

//-----------------------------------------------------------------------------
void pqIntRangeWidget::sliderChanged(int val)
{
  if(!this->BlockUpdate)
    {
    this->BlockUpdate = true;
    this->LineEdit->setText(QString().setNum(val));
    this->setValue(val);
    this->BlockUpdate = false;
    }
}

//-----------------------------------------------------------------------------
void pqIntRangeWidget::textChanged(const QString& text)
{
  if(!this->BlockUpdate)
    {
    int val = text.toInt();
    this->BlockUpdate = true;
    this->Slider->setValue(val);
    this->setValue(val);
    this->BlockUpdate = false;
    }
}

