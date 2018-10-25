#ifndef POINCARE_MATRIX_H
#define POINCARE_MATRIX_H

#include <poincare/dynamic_hierarchy.h>
#include <poincare/matrix_data.h>

namespace Poincare {

class Multiplication;

class Matrix : public DynamicHierarchy {
template<typename T> friend class MatrixComplex;
friend class GlobalContext;
public:
  Matrix(MatrixData * matrixData); // pilfer the operands of matrixData
  Matrix(const Expression * const * operands, int numberOfRows, int numberOfColumns, bool cloneOperands = true);
  Expression * matrixOperand(int i, int j) { return editableOperand(i*numberOfColumns()+j); }
  int numberOfRows() const;
  int numberOfColumns() const;

  /* Expression */
  Type type() const override;
  Expression * clone() const override;
  int writeTextInBuffer(char * buffer, int bufferSize, PrintFloat::Mode floatDisplayMode, int numberOfSignificantDigits) const override;
  int polynomialDegree(char symbolName) const override;

  /* Operation on matrix */
  int rank(Context & context, AngleUnit angleUnit, bool inPlace);
  // Inverse the array in-place. Array has to be given in the form array[row_index][column_index]
  template<typename T> static int ArrayInverse(T * array, int numberOfRows, int numberOfColumns);
#if MATRIX_EXACT_REDUCING
  //template<typename T> Expression * createTrace() const;
  //template<typename T> Expression * createDeterminant() const;
  Matrix * createTranspose() const;
  static Matrix * createIdentity(int dim);
  /* createInverse can be called on any matrix reduce or not, approximate or not. */
  Expression * createInverse(Context & context, AngleUnit angleUnit) const;
#endif
private:
  /* rowCanonize turns a matrix in its reduced row echelon form. */
  void rowCanonize(Context & context, AngleUnit angleUnit, Multiplication * m = nullptr);
  // Row canonize the array in place
  template<typename T> static void ArrayRowCanonize(T * array, int numberOfRows, int numberOfColumns, T * c = nullptr);
  /* Layout */
  ExpressionLayout * createLayout(PrintFloat::Mode floatDisplayMode, int numberOfSignificantDigits) const override;
  /* Evaluation */
  Evaluation<float> * privateApproximate(SinglePrecision p, Context& context, AngleUnit angleUnit) const override { return templatedApproximate<float>(context, angleUnit); }
  Evaluation<double> * privateApproximate(DoublePrecision p, Context& context, AngleUnit angleUnit) const override { return templatedApproximate<double>(context, angleUnit); }
 template<typename T> Evaluation<T> * templatedApproximate(Context& context, AngleUnit angleUnit) const;
  int m_numberOfRows;
};

}

#endif
