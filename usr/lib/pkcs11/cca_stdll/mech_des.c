/*
 * Licensed materials, Property of IBM Corp.
 *
 * openCryptoki CCA token
 *
 * (C) COPYRIGHT International Business Machines Corp. 2001, 2002, 2006
 *
 */


// File:  mech_des.c
//
// Mechanisms for DES
//

#include <string.h>            // for memcmp() et al
#include <stdlib.h>

#include "cca_stdll.h"

#include "pkcs11types.h"
#include "defs.h"
#include "host_defs.h"
#include "h_extern.h"
#include "tok_spec_struct.h"


#ifndef NOECB
//
//
CK_RV
pk_des_ecb_encrypt( SESSION           *sess,
                 CK_BBOOL           length_only,
                 ENCR_DECR_CONTEXT *ctx,
                 CK_BYTE           *in_data,
                 CK_ULONG           in_data_len,
                 CK_BYTE           *out_data,
                 CK_ULONG          *out_data_len )
{
   OBJECT       *key       = NULL;
   CK_ATTRIBUTE *attr      = NULL;
   CK_RV         rc;


   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   // CKM_DES_ECB requires the input data to be an integral
   // multiple of the block size
   //
   if (in_data_len % DES_BLOCK_SIZE != 0){
      st_err_log(109, __FILE__, __LINE__);
      return CKR_DATA_LEN_RANGE;
   }
   rc = object_mgr_find_in_map1( ctx->key, &key );
   if (rc != CKR_OK){
      st_err_log(110, __FILE__, __LINE__);
      return rc;
   }
   rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
   if (rc == FALSE){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   if (length_only == TRUE) {
      *out_data_len = in_data_len;
      return CKR_OK;
   }

   if (*out_data_len < in_data_len) {
      *out_data_len = in_data_len;
      st_err_log(111, __FILE__, __LINE__);
      return CKR_BUFFER_TOO_SMALL;
   }

   return ckm_des_ecb_encrypt( in_data,  in_data_len,
                               out_data, out_data_len, attr->pValue);
}


//
//
CK_RV
des_ecb_decrypt( SESSION           *sess,
                 CK_BBOOL           length_only,
                 ENCR_DECR_CONTEXT *ctx,
                 CK_BYTE           *in_data,
                 CK_ULONG           in_data_len,
                 CK_BYTE           *out_data,
                 CK_ULONG          *out_data_len)
{
   OBJECT       *key       = NULL;
   CK_ATTRIBUTE *attr      = NULL;
   CK_RV         rc;


   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }

   // CKM_DES_ECB requires the input data to be an integral
   // multiple of the block size
   //
   if (in_data_len % DES_BLOCK_SIZE != 0){
      st_err_log(112, __FILE__, __LINE__);
      return CKR_ENCRYPTED_DATA_LEN_RANGE;
   }
   rc = object_mgr_find_in_map1( ctx->key, &key );
   if (rc != CKR_OK){
      st_err_log(110, __FILE__, __LINE__);
      return rc;
   }
   rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
   if (rc == FALSE){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   if (length_only == TRUE) {
      *out_data_len = in_data_len;
      return CKR_OK;
   }

   if (*out_data_len < in_data_len) {
      *out_data_len = in_data_len;
      st_err_log(111, __FILE__, __LINE__);
      return CKR_BUFFER_TOO_SMALL;
   }

   return ckm_des_ecb_decrypt( in_data,  in_data_len,
                               out_data, out_data_len, attr->pValue );
}
#endif

//
//
CK_RV
pk_des_cbc_encrypt( SESSION           *sess,
                 CK_BBOOL           length_only,
                 ENCR_DECR_CONTEXT *ctx,
                 CK_BYTE           *in_data,
                 CK_ULONG           in_data_len,
                 CK_BYTE           *out_data,
                 CK_ULONG          *out_data_len)
{
   OBJECT       *key       = NULL;
   CK_ATTRIBUTE *attr      = NULL;
   CK_RV         rc;

   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   // CKM_DES_CBC requires the input data to be an integral
   // multiple of the block size
   //
   if (in_data_len % DES_BLOCK_SIZE != 0){
      st_err_log(109, __FILE__, __LINE__);
      return CKR_DATA_LEN_RANGE;
   }
   rc = object_mgr_find_in_map1( ctx->key, &key );
   if (rc != CKR_OK){
      st_err_log(110, __FILE__, __LINE__);
      return rc;
   }
   rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
   if (rc == FALSE){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   if (length_only == TRUE) {
      *out_data_len = in_data_len;
      return CKR_OK;
   }

   if (*out_data_len < in_data_len) {
      *out_data_len = in_data_len;
      st_err_log(111, __FILE__, __LINE__);
      return CKR_BUFFER_TOO_SMALL;
   }

   return ckm_des_cbc_encrypt( in_data,  in_data_len,
                               out_data, out_data_len,
                               ctx->mech.pParameter,
                               attr->pValue );
}


//
//
CK_RV
des_cbc_decrypt( SESSION            *sess,
                 CK_BBOOL            length_only,
                 ENCR_DECR_CONTEXT  *ctx,
                 CK_BYTE            *in_data,
                 CK_ULONG            in_data_len,
                 CK_BYTE            *out_data,
                 CK_ULONG           *out_data_len)
{
   OBJECT    *key       = NULL;
   CK_ATTRIBUTE *attr      = NULL;
   CK_RV      rc;


   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   // CKM_DES_CBC requires the input data to be an integral
   // multiple of the block size
   //
   if (in_data_len % DES_BLOCK_SIZE != 0){
      st_err_log(112, __FILE__, __LINE__);
      return CKR_ENCRYPTED_DATA_LEN_RANGE;
   }
   rc = object_mgr_find_in_map1( ctx->key, &key );
   if (rc != CKR_OK){
      st_err_log(110, __FILE__, __LINE__);
      return rc;
   }
   rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
   if (rc == FALSE){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   if (length_only == TRUE) {
      *out_data_len = in_data_len;
      return CKR_OK;
   }

   if (*out_data_len < in_data_len) {
      *out_data_len = in_data_len;
      st_err_log(111, __FILE__, __LINE__);
      return CKR_BUFFER_TOO_SMALL;
   }

   return ckm_des_cbc_decrypt( in_data,  in_data_len,
                               out_data, out_data_len,
                               ctx->mech.pParameter,
                               attr->pValue );
}


//
//
CK_RV
des_cbc_pad_encrypt( SESSION           *sess,
                     CK_BBOOL           length_only,
                     ENCR_DECR_CONTEXT *ctx,
                     CK_BYTE           *in_data,
                     CK_ULONG           in_data_len,
                     CK_BYTE           *out_data,
                     CK_ULONG          *out_data_len)
{
   OBJECT       *key       = NULL;
   CK_ATTRIBUTE *attr      = NULL;
   CK_BYTE      *clear     = NULL;
   CK_ULONG      padded_len;
   CK_RV         rc;

   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   // DES-CBC-PAD has no input length requirements
   //

   rc = object_mgr_find_in_map1( ctx->key, &key );
   if (rc != CKR_OK){
      st_err_log(110, __FILE__, __LINE__);
      return rc;
   }
   rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr  );
   if (rc == FALSE){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   // compute the output length, accounting for padding
   //
   padded_len = DES_BLOCK_SIZE * (in_data_len / DES_BLOCK_SIZE + 1);

   if (length_only == TRUE) {
      *out_data_len = padded_len;
      return CKR_OK;
   }

   if (*out_data_len < padded_len) {
      *out_data_len = padded_len;
      st_err_log(111, __FILE__, __LINE__);
      return CKR_BUFFER_TOO_SMALL;
   }

   clear = (CK_BYTE *)malloc( padded_len );
   if (!clear){
      ock_log_err(OCK_E_MEM_ALLOC);
      return CKR_HOST_MEMORY;
   }
   memcpy( clear, in_data, in_data_len );

   add_pkcs_padding( clear + in_data_len,
                     DES_BLOCK_SIZE,
                     in_data_len,
                     padded_len );

   rc = ckm_des_cbc_encrypt( clear,    padded_len,
                             out_data, out_data_len,
                             ctx->mech.pParameter,
                             attr->pValue );
   if (rc != CKR_OK)
      st_err_log(113, __FILE__, __LINE__);
   free( clear );
   return rc;
}


//
//
CK_RV
des_cbc_pad_decrypt( SESSION            *sess,
                     CK_BBOOL            length_only,
                     ENCR_DECR_CONTEXT  *ctx,
                     CK_BYTE            *in_data,
                     CK_ULONG            in_data_len,
                     CK_BYTE            *out_data,
                     CK_ULONG           *out_data_len)
{
   OBJECT      *key       = NULL;
   CK_ATTRIBUTE   *attr      = NULL;
   CK_BYTE     *clear     = NULL;
   CK_ULONG     padded_len;
   CK_RV        rc;


   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   //
   // no need to validate the input length since we'll pad as necessary
   //

   rc = object_mgr_find_in_map1( ctx->key, &key );
   if (rc != CKR_OK){
      st_err_log(110, __FILE__, __LINE__);
      return rc;
   }
   rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
   if (rc == FALSE){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   // we're decrypting so even with CBC-PAD, we should have an integral
   // number of block to decrypt
   //
   if (in_data_len % DES_BLOCK_SIZE != 0){
      st_err_log(112, __FILE__, __LINE__);
      return CKR_ENCRYPTED_DATA_LEN_RANGE;
   }
   // the amount of cleartext after stripping the padding will actually be less
   // than the input bytes...
   //
   padded_len = in_data_len;

   if (length_only == TRUE) {
      *out_data_len = padded_len;
      return CKR_OK;
   }

   clear = (CK_BYTE *)malloc( padded_len );
   if (!clear){
      ock_log_err(OCK_E_MEM_ALLOC);
      return CKR_HOST_MEMORY;
   }
   rc = ckm_des_cbc_decrypt( in_data, in_data_len,
                             clear,   &padded_len,
                             ctx->mech.pParameter,
                             attr->pValue );

   if (rc == CKR_OK) {
      strip_pkcs_padding( clear, padded_len, out_data_len );
      memcpy( out_data, clear, *out_data_len );
   }
   else
      st_err_log(114, __FILE__, __LINE__);

   free( clear );
   return rc;
}


#ifndef NOECB
//
//
CK_RV
des_ecb_encrypt_update( SESSION            *sess,
                        CK_BBOOL            length_only,
                        ENCR_DECR_CONTEXT  *ctx,
                        CK_BYTE            *in_data,
                        CK_ULONG            in_data_len,
                        CK_BYTE            *out_data,
                        CK_ULONG           *out_data_len )
{
   DES_CONTEXT  * context   = NULL;
   CK_ATTRIBUTE * attr      = NULL;
   OBJECT       * key       = NULL;
   CK_BYTE      * clear     = NULL;
   CK_ULONG       total, remain, out_len;
   CK_RV          rc;

   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   context = (DES_CONTEXT *)ctx->context;

   total = (context->len + in_data_len);

   if (total < DES_BLOCK_SIZE) {
      if (length_only == FALSE) {
         memcpy( context->data + context->len, in_data, in_data_len );
         context->len += in_data_len;
      }
      *out_data_len = 0;
      return CKR_OK;
   }
   else {
      // we have at least 1 block
      //
      remain  = (total % DES_BLOCK_SIZE);
      out_len = (total - remain);     // should always be at least 1 block

      if (length_only == TRUE) {
         *out_data_len = out_len;
         return CKR_OK;
      }

      rc = object_mgr_find_in_map_nocache( ctx->key, &key );
      if (rc != CKR_OK){
         st_err_log(110, __FILE__, __LINE__);
         return rc;
      }
      rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
      if (rc == FALSE){
         ock_log_err(OCK_E_FUNC);
         return CKR_FUNCTION_FAILED;
      }
      clear  = (CK_BYTE *)malloc( out_len );
      if (!clear){
         ock_log_err(OCK_E_MEM_ALLOC);
         return CKR_HOST_MEMORY;
      }
      // copy any data left over from the previous encryption operation first
      //
      memcpy( clear,                context->data, context->len );
      memcpy( clear + context->len, in_data,       out_len - context->len );

      rc = ckm_des_ecb_encrypt( clear,    out_len,
                                out_data, out_data_len, attr->pValue );
      if (rc == CKR_OK) {
         *out_data_len = out_len;

         // update the context buffer.  we already used the buffer's current
         // contents so we completely overwrite it
         //
         if (remain != 0)
            memcpy( context->data, in_data + (in_data_len - remain), remain );
         context->len = remain;
      }
      else
         st_err_log(115, __FILE__, __LINE__);
      free( clear );
      return rc;
   }

   ock_log_err(OCK_E_FUNC);
   return CKR_FUNCTION_FAILED;  // shouldn't reach this
}


//
//
CK_RV
des_ecb_decrypt_update( SESSION           *sess,
                        CK_BBOOL           length_only,
                        ENCR_DECR_CONTEXT *ctx,
                        CK_BYTE           *in_data,
                        CK_ULONG           in_data_len,
                        CK_BYTE           *out_data,
                        CK_ULONG          *out_data_len )
{
   DES_CONTEXT  * context   = NULL;
   CK_ATTRIBUTE * attr      = NULL;
   OBJECT       * key       = NULL;
   CK_BYTE      * cipher    = NULL;
   CK_ULONG       total, remain, out_len;
   CK_RV          rc;

   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   context = (DES_CONTEXT *)ctx->context;

   total = (context->len + in_data_len);

   if (total < DES_BLOCK_SIZE) {
      if (length_only == FALSE) {
         memcpy( context->data + context->len, in_data, in_data_len );
         context->len += in_data_len;
      }

      *out_data_len = 0;
      return CKR_OK;
   }
   else {
      // we have at least 1 block
      //
      remain  = (total % DES_BLOCK_SIZE);
      out_len = total - remain;

      if (length_only == TRUE) {
         *out_data_len = out_len;
         return CKR_OK;
      }

      rc = object_mgr_find_in_map_nocache( ctx->key, &key );
      if (rc != CKR_OK){
         st_err_log(110, __FILE__, __LINE__);
         return rc;
      }
      rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
      if (rc == FALSE){
         ock_log_err(OCK_E_FUNC);
         return CKR_FUNCTION_FAILED;
      }

      cipher = (CK_BYTE *)malloc( out_len );
      if (!cipher){
         ock_log_err(OCK_E_MEM_ALLOC);
         return CKR_HOST_MEMORY;
      }
      // copy any data left over from the previous decryption operation first
      //
      memcpy( cipher,                context->data, context->len );
      memcpy( cipher + context->len, in_data,       out_len - context->len );

      rc = ckm_des_ecb_decrypt( cipher,   out_len,
                                out_data, out_data_len, attr->pValue );
      if (rc == CKR_OK) {
         *out_data_len = out_len;

         // copy the remaining 'new' input data to the context buffer
         //
         if (remain != 0)
            memcpy( context->data, in_data + (in_data_len - remain), remain );
         context->len = remain;
      }
      else
         st_err_log(116, __FILE__, __LINE__);
      free( cipher );
      return rc;
   }

   ock_log_err(OCK_E_FUNC);
   return CKR_FUNCTION_FAILED;  // shouldn't reach this
}
#endif

//
//
CK_RV
des_cbc_encrypt_update( SESSION           *sess,
                        CK_BBOOL           length_only,
                        ENCR_DECR_CONTEXT *ctx,
                        CK_BYTE           *in_data,
                        CK_ULONG           in_data_len,
                        CK_BYTE           *out_data,
                        CK_ULONG          *out_data_len )
{
   DES_CONTEXT  * context   = NULL;
   CK_ATTRIBUTE * attr      = NULL;
   OBJECT       * key       = NULL;
   CK_BYTE      * clear     = NULL;
   CK_ULONG       total, remain, out_len;
   CK_RV          rc;


   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   context = (DES_CONTEXT *)ctx->context;

   total = (context->len + in_data_len);

   if (total < DES_BLOCK_SIZE) {
      if (length_only == FALSE) {
         memcpy( context->data + context->len, in_data, in_data_len );
         context->len += in_data_len;
      }

      *out_data_len = 0;
      return CKR_OK;
   }
   else {
      // we have at least 1 block
      //
      remain  = (total % DES_BLOCK_SIZE);
      out_len = total - remain;

      if (length_only == TRUE) {
         *out_data_len = out_len;
         return CKR_OK;
      }

      rc = object_mgr_find_in_map_nocache( ctx->key, &key );
      if (rc != CKR_OK){
         st_err_log(110, __FILE__, __LINE__);
         return rc;
      }
      rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
      if (rc == FALSE){
         ock_log_err(OCK_E_FUNC);
         return CKR_FUNCTION_FAILED;
      }

      // these buffers need to be longword aligned
      //
      clear  = (CK_BYTE *)malloc( out_len );
      if (!clear){
         ock_log_err(OCK_E_MEM_ALLOC);
         return CKR_HOST_MEMORY;
      }
      // copy any data left over from the previous encryption operation first
      //
      memcpy( clear,                context->data, context->len );
      memcpy( clear + context->len, in_data,       out_len - context->len );

      rc = ckm_des_cbc_encrypt( clear,    out_len,
                                out_data, out_data_len,
                                ctx->mech.pParameter,
                                attr->pValue );
      if (rc == CKR_OK) {
         *out_data_len = out_len;

         // the new init_v is the last encrypted data block
         //
         memcpy( ctx->mech.pParameter, out_data + (*out_data_len - DES_BLOCK_SIZE), DES_BLOCK_SIZE );

         // copy the remaining 'new' input data to the context buffer
         //
         if (remain != 0)
            memcpy( context->data, in_data + (in_data_len - remain), remain );
         context->len = remain;
      }

      else
         st_err_log(113, __FILE__, __LINE__);
      free( clear );
      return rc;
   }

   ock_log_err(OCK_E_FUNC);
   return CKR_FUNCTION_FAILED;
}


//
//
CK_RV
des_cbc_decrypt_update( SESSION           *sess,
                        CK_BBOOL           length_only,
                        ENCR_DECR_CONTEXT *ctx,
                        CK_BYTE           *in_data,
                        CK_ULONG           in_data_len,
                        CK_BYTE           *out_data,
                        CK_ULONG          *out_data_len )
{
   DES_CONTEXT  * context   = NULL;
   CK_ATTRIBUTE * attr      = NULL;
   OBJECT       * key       = NULL;
   CK_BYTE      * cipher    = NULL;
   CK_ULONG       total, remain, out_len;
   CK_RV          rc;


   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   context = (DES_CONTEXT *)ctx->context;

   total = context->len + in_data_len;

   if (total < DES_BLOCK_SIZE) {
      if (length_only == FALSE) {
         memcpy( context->data + context->len, in_data, in_data_len );
         context->len += in_data_len;
      }

      *out_data_len = 0;
      return CKR_OK;
   }
   else {
      // we have at least 1 block
      //
      remain  = total % DES_BLOCK_SIZE;
      out_len = total - remain;

      if (length_only == TRUE) {
         *out_data_len = out_len;
         return CKR_OK;
      }

      rc = object_mgr_find_in_map_nocache( ctx->key, &key );
      if (rc != CKR_OK){
         st_err_log(110, __FILE__, __LINE__);
         return rc;
      }
      rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
      if (rc == FALSE){
         ock_log_err(OCK_E_FUNC);
         return CKR_FUNCTION_FAILED;
      }

      // these buffers need to be longword aligned
      //
      cipher = (CK_BYTE *)malloc( out_len );
      if (!cipher){
         ock_log_err(OCK_E_MEM_ALLOC);
         return CKR_HOST_MEMORY;
      }
      // copy any data left over from the previous decryption operation first
      //
      memcpy( cipher,                context->data, context->len );
      memcpy( cipher + context->len, in_data,       out_len - context->len );

      rc = ckm_des_cbc_decrypt( cipher,   out_len,
                                out_data, out_data_len,
                                ctx->mech.pParameter,
                                attr->pValue );
      if (rc == CKR_OK) {
         *out_data_len = out_len;

         // the new init_v is the last decrypted data block
         //
         memcpy( ctx->mech.pParameter, cipher + (out_len - DES_BLOCK_SIZE), DES_BLOCK_SIZE );

         // copy the remaining 'new' input data to the context buffer
         //
         if (remain != 0)
            memcpy( context->data, in_data + (in_data_len - remain), remain );
         context->len = remain;
      }

      else
         st_err_log(114, __FILE__, __LINE__);
      free( cipher );
      return rc;
   }

   ock_log_err(OCK_E_FUNC);
   return CKR_FUNCTION_FAILED;
}


//
//
CK_RV
des_cbc_pad_encrypt_update( SESSION           *sess,
                            CK_BBOOL           length_only,
                            ENCR_DECR_CONTEXT *ctx,
                            CK_BYTE           *in_data,
                            CK_ULONG           in_data_len,
                            CK_BYTE           *out_data,
                            CK_ULONG          *out_data_len )
{
   DES_CONTEXT  * context   = NULL;
   CK_ATTRIBUTE * attr      = NULL;
   OBJECT       * key       = NULL;
   CK_BYTE      * clear     = NULL;
   CK_ULONG       total, remain, out_len;
   CK_RV          rc;


   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   context = (DES_CONTEXT *)ctx->context;

   total = (context->len + in_data_len);

   // note, this is subtly different from the other encrypt update routines
   //
   if (total <= DES_BLOCK_SIZE) {
      if (length_only == FALSE) {
         memcpy( context->data + context->len, in_data, in_data_len );
         context->len += in_data_len;
      }

      *out_data_len = 0;
      return CKR_OK;
   }
   else {
      // we have at least 1 block + 1 byte
      //
      remain  = total % DES_BLOCK_SIZE;
      out_len = total - remain;

      if (remain == 0) {
         remain     = DES_BLOCK_SIZE;
         out_len -= DES_BLOCK_SIZE;
      }

      if (length_only == TRUE) {
         *out_data_len = out_len;
         return CKR_OK;
      }

      // at this point, we should have:
      //    1) remain != 0
      //    2) out_len != 0
      //
      rc = object_mgr_find_in_map_nocache( ctx->key, &key );
      if (rc != CKR_OK){
         st_err_log(110, __FILE__, __LINE__);
         return rc;
      }
      rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
      if (rc == FALSE){
         ock_log_err(OCK_E_FUNC);
         return CKR_FUNCTION_FAILED;
      }

      // these buffers need to be longword aligned
      //
      clear = (CK_BYTE *)malloc( out_len );
      if (!clear){
         ock_log_err(OCK_E_MEM_ALLOC);
         return CKR_HOST_MEMORY;
      }
      // copy any data left over from the previous encryption operation first
      //
      memcpy( clear,                context->data, context->len );
      memcpy( clear + context->len, in_data,       out_len - context->len );

      //
      // we don't do padding during the update
      //
      rc = ckm_des_cbc_encrypt( clear,    out_len,
                                out_data, out_data_len,
                                ctx->mech.pParameter,
                                attr->pValue );

      if (rc == CKR_OK) {
         // the new init_v is the last encrypted data block
         //
         memcpy( ctx->mech.pParameter, out_data + (*out_data_len - DES_BLOCK_SIZE), DES_BLOCK_SIZE );

         // copy the remaining 'new' input data to the temporary space
         //
         memcpy( context->data, in_data + (in_data_len - remain), remain );
         context->len = remain;
      }


      else
         st_err_log(113, __FILE__, __LINE__);
      free( clear );
      return rc;
   }
}


//
//
CK_RV
des_cbc_pad_decrypt_update( SESSION           *sess,
                            CK_BBOOL           length_only,
                            ENCR_DECR_CONTEXT *ctx,
                            CK_BYTE           *in_data,
                            CK_ULONG           in_data_len,
                            CK_BYTE           *out_data,
                            CK_ULONG          *out_data_len )
{
   DES_CONTEXT  * context   = NULL;
   CK_ATTRIBUTE * attr      = NULL;
   OBJECT       * key       = NULL;
   CK_BYTE      * cipher    = NULL;
   CK_ULONG       total, remain, out_len;
   CK_RV          rc;


   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   context = (DES_CONTEXT *)ctx->context;

   total = (context->len + in_data_len);

   // note, this is subtly different from the other decrypt update routines
   //
   if (total <= DES_BLOCK_SIZE) {
      if (length_only == FALSE) {
         memcpy( context->data + context->len, in_data, in_data_len );
         context->len += in_data_len;
      }

      *out_data_len = 0;
      return CKR_OK;
   }
   else {
      // we have at least 1 block + 1 byte
      //
      remain  = total % DES_BLOCK_SIZE;
      out_len = total - remain;

      if (remain == 0) {
         remain     = DES_BLOCK_SIZE;
         out_len -= DES_BLOCK_SIZE;
      }

      if (length_only == TRUE) {
         *out_data_len = out_len;
         return CKR_OK;
      }

      // at this point, we should have:
      //    1) remain != 0
      //    2) out_len != 0
      //
      rc = object_mgr_find_in_map_nocache( ctx->key, &key );
      if (rc != CKR_OK){
         st_err_log(110, __FILE__, __LINE__);
         return rc;
      }
      rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
      if (rc == FALSE){
         ock_log_err(OCK_E_FUNC);
         return CKR_FUNCTION_FAILED;
      }

      // these buffers need to be longword aligned
      //
      cipher = (CK_BYTE *)malloc( out_len );
      if (!cipher){
         ock_log_err(OCK_E_MEM_ALLOC);
         return CKR_HOST_MEMORY;
      }
      // copy any data left over from the previous decryption operation first
      //
      memcpy( cipher,                context->data, context->len );
      memcpy( cipher + context->len, in_data,       out_len - context->len );

      rc = ckm_des_cbc_decrypt( cipher,  out_len,
                                out_data, out_data_len,
                                ctx->mech.pParameter,
                                attr->pValue );

      if (rc == CKR_OK) {
         // the new init_v is the last decrypted data block
         //
         memcpy( ctx->mech.pParameter, cipher + (out_len - DES_BLOCK_SIZE), DES_BLOCK_SIZE );

         // copy the remaining 'new' input data to the temporary space
         //
         memcpy( context->data, in_data + (in_data_len - remain), remain );
         context->len = remain;
      }
      else
         st_err_log(114, __FILE__, __LINE__);
      free( cipher );
      return rc;
   }

   ock_log_err(OCK_E_FUNC);
   return CKR_FUNCTION_FAILED;
}


#ifndef NOECB
//
//
CK_RV
des_ecb_encrypt_final( SESSION           *sess,
                       CK_BBOOL           length_only,
                       ENCR_DECR_CONTEXT *ctx,
                       CK_BYTE           *out_data,
                       CK_ULONG          *out_data_len )
{
   DES_CONTEXT *context   = NULL;

   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   // satisfy the compiler
   //
   if (length_only)
      context = NULL;

   context = (DES_CONTEXT *)ctx->context;

   // DES-ECB does no padding so there had better not be
   // any data in the context buffer.  if there is it means
   // that the overall data length was not a multiple of the blocksize
   //
   if (context->len != 0){
      st_err_log(109, __FILE__, __LINE__);
      return CKR_DATA_LEN_RANGE;
   }
   *out_data_len = 0;
   return CKR_OK;
}


//
//
CK_RV
des_ecb_decrypt_final( SESSION           *sess,
                       CK_BBOOL           length_only,
                       ENCR_DECR_CONTEXT *ctx,
                       CK_BYTE           *out_data,
                       CK_ULONG          *out_data_len )
{
   DES_CONTEXT *context   = NULL;

   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED; 
   }
   // satisfy the compiler
   //
   if (length_only)
      context = NULL;

   context = (DES_CONTEXT *)ctx->context;

   // DES-ECB does no padding so there had better not be
   // any data in the context buffer.  if there is it means
   // that the overall data length was not a multiple of the blocksize
   //
   if (context->len != 0){
      st_err_log(109, __FILE__, __LINE__);
      return CKR_DATA_LEN_RANGE;
   }
   *out_data_len = 0;
   return CKR_OK;
}
#endif

//
//
CK_RV
des_cbc_encrypt_final( SESSION           *sess,
                       CK_BBOOL           length_only,
                       ENCR_DECR_CONTEXT *ctx,
                       CK_BYTE           *out_data,
                       CK_ULONG          *out_data_len )
{
   DES_CONTEXT *context   = NULL;

   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   // satisfy the compiler
   //
   if (length_only)
      context = NULL;

   context = (DES_CONTEXT *)ctx->context;

   // DES-CBC does no padding so there had better not be
   // any data in the context buffer.  if there is it means
   // that the overall data length was not a multiple of the blocksize
   //
   if (context->len != 0){
      st_err_log(109, __FILE__, __LINE__);
      return CKR_DATA_LEN_RANGE;
   }
   *out_data_len = 0;
   return CKR_OK;
}


//
//
CK_RV
des_cbc_decrypt_final( SESSION           *sess,
                       CK_BBOOL           length_only,
                       ENCR_DECR_CONTEXT *ctx,
                       CK_BYTE           *out_data,
                       CK_ULONG          *out_data_len )
{
   DES_CONTEXT *context   = NULL;

   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   // satisfy the compiler
   //
   if (length_only)
      context = NULL;

   context = (DES_CONTEXT *)ctx->context;

   // DES-CBC does no padding so there had better not be
   // any data in the context buffer.  if there is it means
   // that the overall data length was not a multiple of the blocksize
   //
   if (context->len != 0){
      st_err_log(109, __FILE__, __LINE__);
      return CKR_DATA_LEN_RANGE;
   }
   *out_data_len = 0;
   return CKR_OK;
}


//
//
CK_RV
des_cbc_pad_encrypt_final( SESSION           *sess,
                           CK_BBOOL           length_only,
                           ENCR_DECR_CONTEXT *ctx,
                           CK_BYTE           *out_data,
                           CK_ULONG          *out_data_len )
{
   DES_CONTEXT    *context   = NULL;
   OBJECT         *key       = NULL;
   CK_ATTRIBUTE   *attr      = NULL;
   CK_BYTE         clear[2 * DES_BLOCK_SIZE];
   CK_ULONG        out_len;
   CK_RV           rc;


   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }

   rc = object_mgr_find_in_map1( ctx->key, &key );
   if (rc != CKR_OK){
      st_err_log(110, __FILE__, __LINE__);
      return rc;
   }
   rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
   if (rc == FALSE){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   context = (DES_CONTEXT *)ctx->context;

   // there will never be more than one block in the context buffer
   // so the amount of output is as follows:
   //    if less than 1 block stored, we generate one block of output
   //    if a full block is stored, we generate two blocks of output (one pad block)
   //
   if (context->len == DES_BLOCK_SIZE)
      out_len = 2 * DES_BLOCK_SIZE;
   else
      out_len = DES_BLOCK_SIZE;

   if (length_only == TRUE) {
      *out_data_len = out_len;
      return CKR_OK;
   }
   else {
      memcpy( clear, context->data, context->len );

      add_pkcs_padding( clear + context->len,
                        DES_BLOCK_SIZE,
                        context->len,
                        out_len );

      rc = ckm_des_cbc_encrypt( clear,    out_len,
                                out_data, out_data_len,
                                ctx->mech.pParameter,
                                attr->pValue );
      if (rc != CKR_OK)
         st_err_log(113, __FILE__, __LINE__);
      return rc;
   }
}


//
//
CK_RV
des_cbc_pad_decrypt_final( SESSION           *sess,
                           CK_BBOOL           length_only,
                           ENCR_DECR_CONTEXT *ctx,
                           CK_BYTE           *out_data,
                           CK_ULONG          *out_data_len )
{
   DES_CONTEXT  *context   = NULL;
   OBJECT       *key       = NULL;
   CK_ATTRIBUTE *attr      = NULL;
   CK_BYTE       clear[DES_BLOCK_SIZE];
   CK_BYTE       cipher[DES_BLOCK_SIZE];
   CK_ULONG      out_len;
   CK_RV         rc;

   if (!sess || !ctx || !out_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   rc = object_mgr_find_in_map1( ctx->key, &key );
   if (rc != CKR_OK){
      st_err_log(110, __FILE__, __LINE__);
      return rc;
   }
   rc = template_attribute_find( key->template, CKA_IBM_OPAQUE, &attr );
   if (rc == FALSE){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   context = (DES_CONTEXT *)ctx->context;

   // there had better be a full block in the context buffer
   //
   if (context->len != DES_BLOCK_SIZE){
      st_err_log(112, __FILE__, __LINE__);
      return CKR_ENCRYPTED_DATA_LEN_RANGE;
   }
   // we don't know a priori how much data we'll be returning.  we won't
   // know until after we decrypt it and strip the padding.  it's possible
   // that we'll return nothing (the final block might be a padding block).
   //
   out_len = DES_BLOCK_SIZE;  // upper bound on what we'll return

   if (length_only == TRUE) {
      *out_data_len = out_len;
      return CKR_OK;
   }
   else {
      memcpy( cipher, context->data, DES_BLOCK_SIZE );

      rc = ckm_des_cbc_decrypt( cipher, DES_BLOCK_SIZE,
                                clear,  &out_len,
                                ctx->mech.pParameter,
                                attr->pValue );
      if (rc == CKR_OK) {
         strip_pkcs_padding( clear, DES_BLOCK_SIZE, &out_len );

         if (out_len != 0)
            memcpy( out_data, clear, out_len );

         *out_data_len = out_len;
      }
      else
         st_err_log(114, __FILE__, __LINE__);

      return rc;
   }
}


//
// mechanisms
//


//
//
CK_RV
ckm_des_key_gen( TEMPLATE *tmpl )
{

   CK_ATTRIBUTE     * value_attr    = NULL;
   CK_ATTRIBUTE     * opaque_attr   = NULL;
   CK_ATTRIBUTE     * key_type_attr = NULL;
   CK_ATTRIBUTE     * class_attr    = NULL;
   CK_ATTRIBUTE     * local_attr    = NULL;
   CK_BYTE            dummy_key[DES_KEY_SIZE] = { 0, };
   CK_BYTE            des_key[CCA_KEY_ID_SIZE];
   CK_ULONG           rc;

   rc = token_specific.t_des_key_gen(des_key, CCA_KEY_ID_SIZE, DES_KEY_SIZE);
   if (rc != CKR_OK)
      return rc;

   value_attr    = (CK_ATTRIBUTE *)malloc(sizeof(CK_ATTRIBUTE) + DES_KEY_SIZE );
   opaque_attr   = (CK_ATTRIBUTE *)malloc(sizeof(CK_ATTRIBUTE) + CCA_KEY_ID_SIZE );
   key_type_attr = (CK_ATTRIBUTE *)malloc(sizeof(CK_ATTRIBUTE) + sizeof(CK_KEY_TYPE) );
   class_attr    = (CK_ATTRIBUTE *)malloc(sizeof(CK_ATTRIBUTE) + sizeof(CK_OBJECT_CLASS) );
   local_attr    = (CK_ATTRIBUTE *)malloc(sizeof(CK_ATTRIBUTE) + sizeof(CK_BBOOL) );

   if (!value_attr || !opaque_attr || !key_type_attr || !class_attr || !local_attr) {
      if (value_attr)    free( value_attr );
      if (opaque_attr)   free( opaque_attr );
      if (key_type_attr) free( key_type_attr );
      if (class_attr)    free( class_attr );
      if (local_attr)    free( local_attr );

      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }

   value_attr->type         = CKA_VALUE;
   value_attr->ulValueLen   = DES_KEY_SIZE;
   value_attr->pValue       = (CK_BYTE *)value_attr + sizeof(CK_ATTRIBUTE);
   memcpy( value_attr->pValue, dummy_key, DES_KEY_SIZE );

   opaque_attr->type         = CKA_IBM_OPAQUE;
   opaque_attr->ulValueLen   = CCA_KEY_ID_SIZE;
   opaque_attr->pValue       = (CK_BYTE *)opaque_attr + sizeof(CK_ATTRIBUTE);
   memcpy( opaque_attr->pValue, des_key, CCA_KEY_ID_SIZE );

   key_type_attr->type         = CKA_KEY_TYPE;
   key_type_attr->ulValueLen   = sizeof(CK_KEY_TYPE);
   key_type_attr->pValue       = (CK_BYTE *)key_type_attr + sizeof(CK_ATTRIBUTE);
   *(CK_KEY_TYPE *)key_type_attr->pValue = CKK_DES;

   class_attr->type         = CKA_CLASS;
   class_attr->ulValueLen   = sizeof(CK_OBJECT_CLASS);
   class_attr->pValue       = (CK_BYTE *)class_attr + sizeof(CK_ATTRIBUTE);
   *(CK_OBJECT_CLASS *)class_attr->pValue = CKO_SECRET_KEY;

   local_attr->type         = CKA_LOCAL;
   local_attr->ulValueLen   = sizeof(CK_BBOOL);
   local_attr->pValue       = (CK_BYTE *)local_attr + sizeof(CK_ATTRIBUTE);
   *(CK_BBOOL *)local_attr->pValue = TRUE;

   template_update_attribute( tmpl, value_attr );
   template_update_attribute( tmpl, opaque_attr );
   template_update_attribute( tmpl, key_type_attr );
   template_update_attribute( tmpl, class_attr );
   template_update_attribute( tmpl, local_attr );

   return CKR_OK;
}

#if !(NOCDMF)

//
//
CK_RV
ckm_cdmf_key_gen( TEMPLATE *tmpl )
{
   CK_ATTRIBUTE     * value_attr    = NULL;
   CK_ATTRIBUTE     * key_type_attr = NULL;
   CK_ATTRIBUTE     * class_attr    = NULL;
   CK_ATTRIBUTE     * local_attr    = NULL;
   CK_BYTE            cdmf_key[DES_KEY_SIZE];
   CK_ULONG           repl_len, expected_repl_len;
   CK_ULONG           rc;

   repl_len = expected_repl_len = DES_KEY_SIZE;

   rc = communicate( PK_CDMF_KEYGEN,
                     NULL,       0,
                     cdmf_key,  &repl_len,
                     NULL,       0,
                     NULL,       0 );

   if (rc == CKR_OK) {
      if (repl_len != expected_repl_len)
         return CKR_GENERAL_ERROR;
   }

   value_attr    = (CK_ATTRIBUTE *)malloc(sizeof(CK_ATTRIBUTE) + DES_KEY_SIZE );
   key_type_attr = (CK_ATTRIBUTE *)malloc(sizeof(CK_ATTRIBUTE) + sizeof(CK_KEY_TYPE) );
   class_attr    = (CK_ATTRIBUTE *)malloc(sizeof(CK_ATTRIBUTE) + sizeof(CK_OBJECT_CLASS) );
   local_attr    = (CK_ATTRIBUTE *)malloc(sizeof(CK_ATTRIBUTE) + sizeof(CK_BBOOL) );

   if (!value_attr || !key_type_attr || !class_attr || !local_attr) {
      if (value_attr)    free( value_attr );
      if (key_type_attr) free( key_type_attr );
      if (class_attr)    free( class_attr );
      if (local_attr)    free( local_attr );

      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }


   value_attr->type         = CKA_VALUE;
   value_attr->ulValueLen   = DES_KEY_SIZE;
   value_attr->pValue       = (CK_BYTE *)value_attr + sizeof(CK_ATTRIBUTE);
   memcpy( value_attr->pValue, cdmf_key, DES_KEY_SIZE );

   key_type_attr->type         = CKA_KEY_TYPE;
   key_type_attr->ulValueLen   = sizeof(CK_KEY_TYPE);
   key_type_attr->pValue       = (CK_BYTE *)key_type_attr + sizeof(CK_ATTRIBUTE);
   *(CK_KEY_TYPE *)key_type_attr->pValue = CKK_CDMF;

   class_attr->type         = CKA_CLASS;
   class_attr->ulValueLen   = sizeof(CK_OBJECT_CLASS);
   class_attr->pValue       = (CK_BYTE *)class_attr + sizeof(CK_ATTRIBUTE);
   *(CK_OBJECT_CLASS *)class_attr->pValue = CKO_SECRET_KEY;

   local_attr->type         = CKA_LOCAL;
   local_attr->ulValueLen   = sizeof(CK_BBOOL);
   local_attr->pValue       = (CK_BYTE *)local_attr + sizeof(CK_ATTRIBUTE);
   *(CK_BBOOL *)local_attr->pValue = TRUE;

   template_update_attribute( tmpl, value_attr );
   template_update_attribute( tmpl, key_type_attr );
   template_update_attribute( tmpl, class_attr );
   template_update_attribute( tmpl, local_attr );

   return CKR_OK;
}

#endif

#ifndef NOECB
//
//
CK_RV
ckm_des_ecb_encrypt( CK_BYTE   * in_data,
                     CK_ULONG    in_data_len,
                     CK_BYTE   * out_data,
                     CK_ULONG  * out_data_len,
                     CK_BYTE   * key_value )
{
   CK_ULONG       rc;


   if (!in_data || !out_data || !key_value){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   if (*out_data_len < in_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   rc = token_specific.t_des_ecb(in_data,in_data_len,out_data,out_data_len,key_value,1);//  token specifics return CKR_ errors ... 

   if (rc != CKR_OK)
      st_err_log(117, __FILE__, __LINE__);
   return rc;
}


//
//
CK_RV
ckm_des_ecb_decrypt( CK_BYTE   * in_data,
                     CK_ULONG    in_data_len,
                     CK_BYTE   * out_data,
                     CK_ULONG  * out_data_len,
                     CK_BYTE   * key_value )
{
   CK_ULONG         rc;



   if (!in_data || !out_data || !key_value){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   if (*out_data_len < in_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }

   rc = token_specific.t_des_ecb(in_data,in_data_len,out_data,
         out_data_len,key_value,0);  // last parm is the encrypt flag
   if (rc != CKR_OK)
      st_err_log(117, __FILE__, __LINE__);
   return rc;
}
#endif

//
//
CK_RV
ckm_des_cbc_encrypt( CK_BYTE   * in_data,
                     CK_ULONG    in_data_len,
                     CK_BYTE   * out_data,
                     CK_ULONG  * out_data_len,
                     CK_BYTE   * init_v,
                     CK_BYTE   * key_value )
{
   CK_ULONG         rc;


   if (!in_data || !out_data || !init_v || !key_value){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   if (*out_data_len < in_data_len){
#if 0
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
#else
      *out_data_len = in_data_len;
      st_err_log(68, __FILE__, __FUNCTION__);
      return CKR_BUFFER_TOO_SMALL;
#endif
   }
   rc = token_specific.t_des_cbc(in_data,in_data_len,out_data,
         out_data_len,key_value,init_v,1);  // last parm is the encrypt flag

   if (rc != CKR_OK)
      st_err_log(118, __FILE__, __LINE__);
   return rc;
}


//
//
CK_RV
ckm_des_cbc_decrypt( CK_BYTE   * in_data,
                     CK_ULONG    in_data_len,
                     CK_BYTE   * out_data,
                     CK_ULONG  * out_data_len,
                     CK_BYTE   * init_v,
                     CK_BYTE   * key_value )
{
   CK_ULONG         rc;


   if (!in_data || !out_data || !init_v || !key_value){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }
   if (*out_data_len < in_data_len){
      ock_log_err(OCK_E_FUNC);
      return CKR_FUNCTION_FAILED;
   }

   rc = token_specific.t_des_cbc(in_data,in_data_len,out_data,
         out_data_len,key_value,init_v,0);  // last parm is the encrypt flag

   if (rc != CKR_OK)
      st_err_log(118, __FILE__, __LINE__);
   return rc;
}


// we're preparing to wrap a key using DES.  need to make sure the
// data length is a integral multiple of the DES block size.
//
// PKCS #11 specifies that the padding shall be in the form of NULL
// bytes appended to the data
//
// this routine works with either DES and 3DES as the wrapping mechanism
//
CK_RV
ckm_des_wrap_format( CK_BBOOL    length_only,
                     CK_BYTE  ** data,
                     CK_ULONG  * data_len )
{
   CK_BYTE   * ptr = NULL;
   CK_ULONG    len1, len2;


   len1 = *data_len;

   // if the input key data isn't a multiple of the blocksize,
   // we pad with NULLs to the next blocksize multiple.
   //
   if (len1 % DES_BLOCK_SIZE != 0) {
      len2 = DES_BLOCK_SIZE * ((len1 / DES_BLOCK_SIZE) + 1);

      if (length_only == FALSE) {
         ptr = (CK_BYTE *)realloc(*data, len2);
         if (!ptr){
            ock_log_err(OCK_E_MEM_ALLOC);
            return CKR_HOST_MEMORY;
         }
         else
            memset( ptr + len1, 0x0, (len2 - len1) );

         *data     = ptr;
         *data_len = len2;
      }
   }

   return CKR_OK;
}
