

#include "mlx-sos-filter.h"


namespace mlx
{


    MlxSOSFilterStage::MlxSOSFilterStage(double b0, double b1, double b2, double a1, double a2)
    : _b0(b0), _b1(b1), _b2(b2), _a0(1), _a1(a1), _a2(a2)
    {
        reset();
    }


    MlxSOSFilterStage::~MlxSOSFilterStage()
    {
    }


    void MlxSOSFilterStage::reset()
    {
        _t0 = 0.0;
        _t1 = 0.0;
    }


    double MlxSOSFilterStage::process(double sample)
    {
        double out = 0.0;

        out = _t0 + (_b0 * sample);
        _t0 = _t1 + (_b1 * sample) - (_a1 * out);
        _t1 = (_b2 * sample) - (_a2 * out);

        return out;
    }



/// Start - MlxSOSFilter

    MlxSOSFilter::MlxSOSFilter()
    {
    }


    MlxSOSFilter::~MlxSOSFilter()
    {
    }


    double MlxSOSFilter::filter(double sample)
    {
        double temp = sample;

        for (MlxSOSFilterStage& fil : _filterSet)
        {
            temp = fil.process(temp);
        }

        return temp;
    }


    MlxSOSFilter* MlxSOSFilter::addStage(double b0, double b1, double b2, double a1, double a2)
    {
        _filterSet.push_back(
            MlxSOSFilterStage(b0, b1, b2, a1, a2)
        );

        return this;
    }


/// END - MlxSOSFilter
/// Start - MlxSOSFilterFactory


    MlxSOSFilterFactory::MlxSOSFilterFactory()
    {
    }


    MlxSOSFilterFactory::~MlxSOSFilterFactory()
    {
    }


    std::shared_ptr<MlxSOSFilter> MlxSOSFilterFactory::getFilter_Butterworth(fc_fs_ratio fcsr)
    {
        std::shared_ptr<MlxSOSFilter> filter = std::make_shared<MlxSOSFilter>();


        switch (fcsr)
        {

        case ratio_10p:
            filter->addStage(
                5.128132645416496e-13, 1.0256265290832993e-12, 5.128132645416496e-13, -1.6358161595128016, 0.6694470835583565
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.6544507637455192, 0.6884647986656827
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.6916794338709686, 0.7264588571081657
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.7472829046575309, 0.7832054857562134
            )
            ->addStage(
                1.0, 2.0, 1.0,  -1.8205716411956112, 0.8580009734763099
            )
            ->addStage(
                1.0, 2.0, 1.0,  -1.9099233994325144, 0.9491897243221484
            );

            break;
        
        default:
            break;
        }


        return filter;
    }


    std::shared_ptr<MlxSOSFilter> MlxSOSFilterFactory::getFilter_Bessel(fc_fs_ratio fcsr)
    {
        std::shared_ptr<MlxSOSFilter> filter = std::make_shared<MlxSOSFilter>();

        switch (fcsr)
        {
        
        case ratio_05p:

            filter->addStage(
                1.3164591487194416e-12, 2.632918297438883e-12, 1.3164591487194416e-12, -1.4967220429743726, 0.5604402298082104
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.5031754338000556, 0.5684943341991748
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.5165678617134124, 0.5852950144369798
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.5380229114586013, 0.6124571676498912
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.5697380725036985, 0.6531803028874344
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.6162389834768125, 0.7141683430722784
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.6894048345910613, 0.8133772402519509
            );

            break;

        case ratio_10p:
            filter->addStage(
                6.346130800455662e-09, 1.2692261600911324e-08, 6.346130800455662e-09, -1.0875854483710024, 0.2967242585300517
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.0931310231047813, 0.3080182221218568
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.1045919801935986, 0.3317747936513281
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.1228090927121035, 0.37074660344375854
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.1493766733713506, 0.43047508936230816
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.1874264993018948, 0.5228171438494588
            )
            ->addStage(
                1.0, 2.0, 1.0, -1.244638184591471, 0.6803072537872193
            );

            break;
        
        default:
            break;
        }


        return filter;
    }



/// END - MlxSOSFilterFactory

/*    

float kms_FilterSample(float sample, filter_stage *filter, int stages) {

    float in = sample;
    float out = 0;

    for (int n = 0; n < stages; n++)
    {
        filter_stage *filt = &filter[n];

        out = filt->t[0] + (filt->b[0] * in);
        filt->t[0] = filt->t[1] + (filt->b[1] * in) - (filt->a[1] * out);
        filt->t[1] = (filt->b[2] * in) - (filt->a[2] * out);

        in = out;
    }

    return out;
}
*/

}   /* namespace mlx */