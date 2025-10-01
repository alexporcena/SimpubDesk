#ifndef __SIMPUBLI_AUDESP_RECEITAS_H__
#define __SIMPUBLI_AUDESP_RECEITAS_H__

#include "simpubli_base_audesp.h"

class SimpubliAudespReceitas : public SimpubliBaseAudesp {
public:
	SimpubliAudespReceitas();
	~SimpubliAudespReceitas() {};
protected:
	virtual void InsertDatabase();
};


#endif // !__SIMPUBLI_AUDESP_RECEITAS_H__
