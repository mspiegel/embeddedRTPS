/*
 *
 * Author: Andreas Wüstenberg (andreas.wuestenberg@rwth-aachen.de)
 */

#ifndef RTPS_BUILTINENDPOINTS_H
#define RTPS_BUILTINENDPOINTS_H

#include "rtps/entities/StatelessWriter.h"
#include "rtps/entities/StatelessReader.h"
#include "rtps/entities/StatefullReader.h"
#include "rtps/entities/Writer.h"

namespace rtps{
    struct BuiltInEndpoints{
        StatelessWriter* spdpWriter;
        StatelessReader* spdpReader;
        Writer* sedpPubWriter;
        StatefullReader* sedpPubReader;
        Writer* sedpSubWriter;
        StatefullReader* sedpSubReader;
    };
}

#endif //RTPS_BUILTINENDPOINTS_H