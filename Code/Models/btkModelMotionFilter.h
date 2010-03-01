/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
 
#ifndef __btkModelMotionFilter_h
#define __btkModelMotionFilter_h

#include "btkProcessObject.h"
#include "btkAcquisition.h"
#include "btkModel.h"
#include "btkGeometry.h"
#include "btkLandmarksTranslator.h"
#include "btkMotionBuilder.h"

#include <map>

namespace btk
{
  class ModelMotionFilter : public ProcessObject
  {
  public:
    typedef SharedPtr<ModelMotionFilter> Pointer;
    typedef SharedPtr<const ModelMotionFilter> ConstPointer;

    static Pointer New() {return Pointer(new ModelMotionFilter());};
    
    // ~ModelMotionFilter(); // Implicit
    
    void SetAcquisition(Acquisition::Pointer input) {this->SetNthInput(0, input);};
    Acquisition::Pointer GetAcquisition() {return static_pointer_cast<Acquisition>(this->GetNthInput(0));};    
    void SetModel(Model::Pointer input) {this->SetNthInput(1, input);};
    Model::Pointer GetModel() {return static_pointer_cast<Model>(this->GetNthInput(1));};
    void SetLandmarksTranslator(LandmarksTranslator::Pointer input) {this->SetNthInput(2, input);};
    LandmarksTranslator::Pointer GetLandmarksTranslator() {return static_pointer_cast<LandmarksTranslator>(this->GetNthInput(2));};
    Model::Pointer GetOutput() {return this->GetOutput(0);};
    
    BTK_MODELS_EXPORT void SetMotionBuilder(MotionBuilder::Pointer builder);
    MotionBuilder::Pointer GetMotionBuilder() {return this->m_MotionBuilder;};

  protected:
    BTK_MODELS_EXPORT ModelMotionFilter();
    
    Model::Pointer GetOutput(int idx) {return static_pointer_cast<Model>(this->GetNthOutput(idx));};
    BTK_MODELS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_MODELS_EXPORT virtual void GenerateData();
    
  private:
    ModelMotionFilter(const ModelMotionFilter& ); // Not implemented.
    ModelMotionFilter& operator=(const ModelMotionFilter& ); // Not implemented.
    
    MotionBuilder::Pointer m_MotionBuilder;
  };
};

#endif // __btkModelMotionFilter_h