
#include "make_init.hpp"


namespace hisi
{
    namespace make
    {
        namespace help_
        {


eoInit<pos::single_t>& _make_attr_init_n(
        const eoParamParamType& ppt, std::string name,
        eoState &state, const DataInfo& info)
{
    try {
        auto pr = parseRange<pos::single_t>(
                ppt, -info.maxFlowLength, info.maxFlowLength);

        if(std::get<0>(pr) == std::string("Uni")) {
            IntegerInitUniform<pos::single_t>* init =
                new IntegerInitUniform<pos::single_t>(
                        std::get<1>(pr), std::get<2>(pr));
            state.storeFunctor(init);
            return *init;
        }

        throw InputParamError("Init: unknown "+name+" initialization");

    } catch (std::invalid_argument ia) {
        throw InputParamError("Init: "+name+" - wrong range pram value: "+
                std::string(ia.what()));
    }
}

// make_int
template<>
eoInit<pos::single_t>& make_attr_init<pos::single_t>(
        eoParser& parser, eoState &state, const DataInfo& info)
{
    using namespace make_param_;
    eoParamParamType attr =
        INIT_ATTR_1.doGetORcreateParam(parser).value();
    return _make_attr_init_n(attr, "arg1", state, info);
}

// make_array
template<>
eoInit<pos::dual_t>& make_attr_init<pos::dual_t>(
        eoParser& parser, eoState &state, const DataInfo& info)
{
    using namespace make_param_;
    eoParamParamType attr1 =
        INIT_ATTR_1.doGetORcreateParam(parser).value();
    eoInit<pos::single_t>& arg1  = _make_attr_init_n(attr1, "arg1", state, info);

    using namespace make_param_;
    eoParamParamType attr2 =
        INIT_ATTR_1.doGetORcreateParam(parser).value();
    eoInit<pos::single_t>& arg2  = _make_attr_init_n(attr2, "arg2", state, info);

    eoInit<pos::dual_t>* init = new ArrayInit<pos::dual_t>(
            std::array<eoInit<pos::single_t>*, 2>{{&arg1, &arg2}});
    state.storeFunctor(init);
    return *init;
}

        } /* help_ */
    } /* make */
} /* hisi */

