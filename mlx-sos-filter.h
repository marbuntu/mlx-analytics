


#pragma once

#include <vector>
#include <memory>


namespace mlx
{

/**
 * @brief 
 * 
 */
class MlxSOSFilterStage
{
public:

    /**
     * @brief   SOS Filter Stage - Coefficients in Transposed Direct Form II
     *  
     * @param b0    Nominator Coeff b0 in TDF II
     * @param b1    Nominator Coeff b1 in TDF II
     * @param b2    Nominator Coeff b2 in TDF II
     * @param a1    Denominator Coeff a1 in TDF II
     * @param a2    Denominator Coeff a2 in TDF II
     */
    MlxSOSFilterStage(double b0, double b1, double b2, double a1, double a2);
    
    ~MlxSOSFilterStage();

    /**
     * @brief   Processe one Sample
     * 
     * @param sample  Sample
     * @return double 
     */
    double process(double sample);


    /**
     * @brief   Reset internal Mem-Elements to zero
     * 
     */
    void reset();


protected:
private:

    // Coefficient Set - Denominator
    double _a0;
    double _a1;
    double _a2;

    // Coefficient Set - Nominator
    double _b0;
    double _b1;
    double _b2;

    // Memory Elements
    double _t0;
    double _t1;

};  /* MlxFilterButterworth */



class MlxSOSFilter
{
public:
    MlxSOSFilter();
    ~MlxSOSFilter();

    double filter(double sample);

    MlxSOSFilter* addStage(double b0, double b1, double b2, double a1, double a2);


protected:

    std::vector<MlxSOSFilterStage> _filterSet;


};  /* MlxSOSFilter */



class MlxSOSFilterFactory
{
public:
    typedef enum 
    {
        ratio_05p = 1,
        ratio_08  = 2,
        ratio_10p = 3,
    } fc_fs_ratio;


    MlxSOSFilterFactory();
    ~MlxSOSFilterFactory();

    
    static std::shared_ptr<MlxSOSFilter> getFilter_Butterworth(fc_fs_ratio fcsr);

    /**
     * @brief   Returns Digital Bessel Filter of 14th Order with fc relative to fs
     * 
     * @param   fcsr    Ratio Cutoff to Sample Frequency 
     * @return  std::shared_ptr<MlxSOSFilter> 
     */
    static std::shared_ptr<MlxSOSFilter> getFilter_Bessel(fc_fs_ratio fcsr);

protected:

};  /* MlxSOSFilterFactory */


}   /* namespace mlx */