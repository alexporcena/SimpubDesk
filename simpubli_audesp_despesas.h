#ifndef __SIMPUBLI_AUDESP_DESPESAS_H__
#define __SIMPUBLI_AUDESP_DESPESAS_H__

#include "simpubli_base_audesp.h"

class SimpubliAudespDespesas : public SimpubliBaseAudesp {
public:
	SimpubliAudespDespesas();
	~SimpubliAudespDespesas() {};
protected:
	virtual void InsertDatabase();
};


#endif // !__SIMPUBLI_AUDESP_DESPESAS_H__
