#ifndef HASHBLOCK_H
#define HASHBLOCK_H

#include "uint256.h"
#include "sph_skein.h"
#include "sph_shavite.h"
#include "sph_shabal.h"
#include "sph_gost.h"

#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL sph_skein512_context     z_skein;
GLOBAL sph_shavite512_context   z_shavite;
GLOBAL sph_shabal512_context      z_shabal;
GLOBAL sph_gost512_context      z_gost;

#define fillz() do { \
    sph_skein512_init(&z_skein); \
    sph_shavite512_init(&z_shavite); \
    sph_shabal512_init(&z_shabal); \
    sph_gost512_init(&z_gost); \
} while (0) 


#define ZSKEIN (memcpy(&ctx_skein, &z_skein, sizeof(z_skein)))
#define ZGOST (memcpy(&ctx_gost, &z_gost, sizeof(z_gost)))

template<typename T1>
inline uint256 Hash9(const T1 pbegin, const T1 pend)

{
    sph_skein512_context     ctx_skein;
    sph_shavite512_context   ctx_shavite;
    sph_shabal512_context      ctx_shabal;
    sph_gost512_context      ctx_gost;
    static unsigned char pblank[1];

#ifndef QT_NO_DEBUG
    //std::string strhash;
    //strhash = "";
#endif
    
    uint512 hash[17];

    sph_skein512_init(&ctx_skein);
    sph_skein512 (&ctx_skein, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
    sph_skein512_close(&ctx_skein, static_cast<void*>(&hash[0]));
    
    sph_shavite512_init(&ctx_shavite);
    sph_shavite512(&ctx_shavite, static_cast<const void*>(&hash[0]), 64);
    sph_shavite512_close(&ctx_shavite, static_cast<void*>(&hash[1]));
        
    sph_shabal512_init(&ctx_shabal);
    sph_shabal512 (&ctx_shabal, static_cast<const void*>(&hash[1]), 64);
    sph_shabal512_close(&ctx_shabal, static_cast<void*>(&hash[2]));

    sph_gost512_init(&ctx_gost);
    sph_gost512 (&ctx_gost, static_cast<const void*>(&hash[2]), 64);
    sph_gost512_close(&ctx_gost, static_cast<void*>(&hash[3]));

    return hash[3].trim256();
}






#endif // HASHBLOCK_H
