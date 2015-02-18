/**
 *       @file  itkDWISingleVoxelGenerator.h
 *      @brief  
 *
 *
 *     @author  Dr. Jian Cheng (JC), jian.cheng.1983@gmail.com
 *
 *   @internal
 *     Created  "01-20-2014
 *    Revision  1.0
 *    Compiler  gcc/g++
 *     Company  IDEA@UNC-CH
 *   Copyright  Copyright (c) 2014, Jian Cheng
 *
 * =====================================================================================
 */

#ifndef __itkDWISingleVoxelGenerator_h
#define __itkDWISingleVoxelGenerator_h

#include "utl.h"
#include "itkDWIGeneratorBase.h"

namespace itk
{

/** \class DWISingleVoxelGenerator
 *  \brief Generate DWI data with the same diffusion parameter set for all voxels.
 *    If random mode is used, the diffusion parameters are randomly rotated
 *
 */
template <class TOutputImage, class TScalarImage=Image<float,3> >
class ITK_EXPORT DWISingleVoxelGenerator : public DWIGeneratorBase<TOutputImage, TScalarImage>
{
public:
  /** Standard class typedefs. */
  typedef DWISingleVoxelGenerator                Self;
  typedef DWIGeneratorBase<TOutputImage, TScalarImage>   Superclass;
  typedef SmartPointer<Self>          Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DWISingleVoxelGenerator, DWIGeneratorBase);

  /** Some convenient typedefs. */
  typedef typename Superclass::OutputImageType                  OutputImageType;
  typedef typename Superclass::OutputImagePointer               OutputImagePointer;
  typedef typename Superclass::OutputImageSizeType              OutputImageSizeType;
  typedef typename Superclass::OutputImageSpacingType           OutputImageSpacingType;
  typedef typename Superclass::OutputImageIndexType             OutputImageIndexType;
  typedef typename Superclass::OutputImagePointType             OutputImagePointType;
  typedef typename Superclass::OutputImageDirectionType         OutputImageDirectionType;
  typedef typename Superclass::OutputImageRegionType            OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType             OutputImagePixelType;
  typedef typename Superclass::OutputImageInternalPixelType     OutputImageInternalPixelType;

  /** Output image dimension */
  itkStaticConstMacro(OutputImageDimension, unsigned int, OutputImageType::ImageDimension);

  /** B0 Image */
  typedef typename Superclass::ScalarImageType           ScalarImageType;
  typedef typename Superclass::ScalarImagePointer        ScalarImagePointer;

  /** Orientation Matrice Type */
  typedef typename Superclass::PrecisionType         PrecisionType;
  typedef typename Superclass::MatrixType            MatrixType;
  typedef typename Superclass::MatrixPointer         MatrixPointer;
  typedef typename Superclass::STDVectorType         STDVectorType;
  typedef typename Superclass::STDVectorPointer      STDVectorPointer;
    
  /** Some convenient typedefs for diffusion parameters. */  
  typedef typename Superclass::DiffusionParameterValuesType    DiffusionParameterValuesType;
  typedef typename Superclass::DiffusionParameterContainerType DiffusionParameterContainerType;
  
  /** \param FIXED fixed fiber configuration for all voxels.
   *  \param UNIFORM If m_StoredOrientationMatrix is not set, then rotate the fiber configruation using a uniformly random rotation matrix.
   *    Otherwise, rotate the fiber configruation using the stored m_StoredOrientationMatrix, and m_OutputSize is overridden by the number of stored orienrations.
   *  \param GAUSSIAN rotate the fiber configruation using a random rotation matrix generated by a Riemannian Gaussian distribution.
   **/
  typedef enum 
    {
    FIXED=0, 
    UNIFORM 
    } RandomType;
  
  itkSetMacro( StoredOrientationMatrix, MatrixPointer);
  itkGetMacro( StoredOrientationMatrix, MatrixPointer );

  void SetUniformFromElectrostaticEnergy(const int num)
    {
    m_StoredOrientationMatrix = utl::ReadGradElectricRepulsion<double>(num,DIRECTION_NODUPLICATE,CARTESIAN_TO_CARTESIAN, DIRECTION_NOFLIP, DIRECTION_NOFLIP, DIRECTION_NOFLIP, true);
    this->Modified();
    }
  void SetUniformFromTessellation(const int num)
    {
    m_StoredOrientationMatrix = utl::ReadGrad<double>(num,DIRECTION_NODUPLICATE,CARTESIAN_TO_CARTESIAN,DIRECTION_NOFLIP, DIRECTION_NOFLIP, DIRECTION_NOFLIP, true);
    this->Modified();
    }


  
  itkSetMacro( RandomType, RandomType );
  itkGetMacro( RandomType, RandomType );

  /** diffusion parameters for all voxels  */
  itkSetMacro( DiffusionParameterValues, DiffusionParameterValuesType );
  itkGetMacro( DiffusionParameterValues, DiffusionParameterValuesType );
  
protected:
  DWISingleVoxelGenerator();
  ~DWISingleVoxelGenerator(){};

  typename LightObject::Pointer InternalClone() const;
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  /** Does the real work. */
  void GenerateData();
  
  void Initialization();
  
  DiffusionParameterValuesType m_DiffusionParameterValues;

  RandomType m_RandomType;

  MatrixPointer m_StoredOrientationMatrix;

private:
  DWISingleVoxelGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  
};

}

#if !defined(ITK_MANUAL_INSTANTIATION) && !defined(__itkDWISingleVoxelGenerator_hxx)
#include "itkDWISingleVoxelGenerator.hxx"
#endif


#endif 
