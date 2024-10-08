#include <GED/Core/CircularQueue.h>
#include <stdlib.h>
#include <string.h>

struct _Header {
    size_t lastIdx;
	size_t size;
};


#define Header ae2f_dynamic_cast(struct _Header*, (_this)->data)
#define NullAssert(cond)  if(!(cond)) return ae2f_errGlobal_PTR_IS_NULL 

#define Buff ((_this)->data + sizeof(struct _Header))
#define Good ae2f_errGlobal_OK;

/// @param _this The constant instance of the class.
/// @param pSize A pointer where the length of the array.
static ae2f_errint_t getsize(
	const ae2f_struct ae2f_ds_Alloc_Refer* _this,
	size_t* pSize
) {
	NullAssert(_this && pSize); // BUG?
	*pSize = Header->size;
	return Good;
}

/// @param _this The constant instance of the class.
/// @param Index The Index [in an unit of bytes] where the element to copy is.
/// @param lpBuff A pointer of buffer where the element to be copied.
/// @param Buff_size The allocated size of the `lpBuff`.
static ae2f_errint_t Read(
	const ae2f_struct ae2f_ds_Alloc_Refer* _this, 
	size_t Index, 
	char* lpBuff, 
	size_t Buff_size
) {
	NullAssert(_this && lpBuff);
	if (Header->size <= Buff_size + Index)
		return ae2f_errGlobal_WRONG_OPERATION;
	for (size_t i=0;i<Buff_size;i++)
		lpBuff[i] = Buff[Index+i];
	return Good;
}

/// @param _this The instance of the class.
/// @param Index The Index [in an unit of bytes] where the element to be overwritten.
/// @param lpBuff A pointer of an element.
/// @param Buff_size An allocated size of the `lpBuff`.
static ae2f_errint_t Write(
	ae2f_struct ae2f_ds_Alloc_Refer* _this, 
	size_t Index, 
	const void* lpBuff,
	size_t Buff_size
) {
	NullAssert(_this && lpBuff);
	if (Header->size <= Buff_size + Index)
		return ae2f_errGlobal_WRONG_OPERATION;
	for (size_t i=0;i<Buff_size;i++)
		Buff[Index+i] = ((const char*) lpBuff)[i];
	return Good;
}

/// @brief 
/// Frees the memory of `_this`.
/// @param _this The instance of a class where the memory to free is located.
static ae2f_errint_t Del(
	ae2f_struct ae2f_ds_Alloc_Owner* _this
) {
	NullAssert(_this);
	return ae2f_errGlobal_IMP_NOT_FOUND;
}

/// @brief Resizes the memory of `_this` in a byte size of `size`.
static ae2f_errint_t Resize(
	ae2f_struct ae2f_ds_Alloc_Owner* _this, 
	size_t size
) {
	NullAssert(_this);

	// if data null,
	// means that we have no allocated memory
	if(_this->data) {
		// We have pointer
		void* try = realloc(_this->data, size);

		if(try) {
			_this->data = try;
			for(size_t i = Header->size; i < size; i++) {
				Buff[i] = 0;
			}

			Header->size = size;
			return Good;
		} else {
			return ae2f_errGlobal_ALLOC_FAILED;
		}
	} {
		_this->data = calloc(size, 1);
		if(_this->data) {
			Header->size = size;
			return Good;
		}
		else return ae2f_errGlobal_ALLOC_FAILED;
	}

	return ae2f_errGlobal_LMT;
}

ae2f_SHAREDEXPORT ae2f_extern const ae2f_struct ae2f_ds_Alloc_vOwner GED_Core_Owner_cCircularQueue = {
	.Del = Del,
	.reSize = Resize
};

ae2f_SHAREDEXPORT ae2f_extern const ae2f_struct ae2f_ds_Alloc_vRefer GED_Core_Refer_cCircularQueue = {
	.getSize = getsize,
	.Read = Read,
	.Write = Write
};