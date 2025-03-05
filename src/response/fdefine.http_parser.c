
//silver_chain_scope_start
//DONT MODIFY THIS COMMENT
//this import is computationally generated
//mannaged by SilverChain: https://github.com/OUIsolutions/SilverChain
#include "../imports/imports.fdeclare.h"
//silver_chain_scope_end


void private_BearHttpsResponse_parse_headers(BearHttpsResponse *self,int headers_end){
    const short WAITING_FIRST_SPACE = 0;
    const short WAITING_STATUS_CODE = 1;
    const short WAITING_FIRST_LINE_TERMINATION  = 3;
    const short COLECTING_KEY = 4;
    const short WAITING_END_KEY = 5;
    const short COLECTING_VAL = 6;
    const short WAITING_END_VAL = 7;
    short state = WAITING_FIRST_SPACE;
    private_BearHttpsKeyVal *current_key_vall = NULL;
    for(int i =3 ; i < headers_end;i++){
        if(self->raw_content[i] == ' ' && state == WAITING_FIRST_SPACE){
            state = WAITING_STATUS_CODE;
            continue;
        }
        if(self->raw_content[i] != ' ' && state == WAITING_STATUS_CODE){
            self->status_code = atoi((const char*)self->raw_content + i);
            state = WAITING_FIRST_LINE_TERMINATION;
            continue;
        }
        if(self->raw_content[i] == '\n' && self->raw_content[i-1] =='\r' && state == WAITING_FIRST_LINE_TERMINATION){
            state = COLECTING_KEY;
            continue;
        }
        if(state == COLECTING_KEY && self->raw_content[i] != ' '){
            current_key_vall = private_newBearHttpsKeyVal();
            private_BearHttpsKeyVal_set_key(current_key_vall,(char*)self->raw_content+i,BEARSSL_HTTPS_REFERENCE);
            state = WAITING_END_KEY;
            continue;
        }

        if(state == WAITING_END_KEY && self->raw_content[i] == ':'){
            self->raw_content[i] = '\0';
            state = COLECTING_VAL;
            continue;
        }
        if(state == COLECTING_VAL && self->raw_content[i] != ' '){
            private_BearHttpsKeyVal_set_value(current_key_vall,(char*)self->raw_content+i,BEARSSL_HTTPS_REFERENCE);
            state = WAITING_END_VAL;
            continue;
        }

        if( self->raw_content[i-1] =='\r' && self->raw_content[i] == '\n'  && state == WAITING_END_VAL){
            self->raw_content[i-1] = '\0';
            private_BearHttpsHeaders_add_keyval(self->headers,current_key_vall);
            current_key_vall = NULL;
            state = COLECTING_KEY;
            continue;
        }

    }

    char *content_length = BearHttpsResponse_get_header_value_by_sanitized_key(self,"contentlength");
    if(content_length != NULL){
        self->user_content_length = atol(content_length);
    }

}
void private_BearHttpsResponse_read_til_end_of_headers_or_reach_limit(
    BearHttpsResponse *self,
    int chunk_size,
    double factor_headers_growth
){


    self->raw_content = (unsigned char *)BearsslHttps_allocate(chunk_size+2);
    long content_allocated  = chunk_size;
    long content_size = 0;
    while(true){
        //apply the factor realloc
        while(content_size + chunk_size >= content_allocated -2 ){

            content_allocated = (long)(content_allocated * factor_headers_growth);
            self->raw_content = (unsigned char*)BearsslHttps_reallocate(self->raw_content,content_allocated);
        }
        //we create a buff str , to allow 'append' into the content
        unsigned char *content_point = (self->raw_content +content_size);

        int readded = private_BearHttpsResponse_read_chunck_raw(self,content_point, chunk_size);
        if(readded == 0){
            BearHttpsResponse_set_error(self,"invalid http response",BEARSSL_HTTPS_INVALID_HTTP_RESPONSE);
            return;

        }

        if(readded < 0){
            char error_buff[100] ={0};
            snprintf(error_buff,sizeof(error_buff),"invalid read code: %d",readded);
            BearHttpsResponse_set_error(self,error_buff,BEARSSL_HTTPS_INVALID_READ_CODE);
            return;
        }


        for(int i = 3; i < readded;i++){
            if(
                content_point[i-3] == '\r' &&
                content_point[i-2] == '\n' &&
                content_point[i-1] == '\r' &&
                content_point[i] == '\n' )
            {

                self->body_start_index =content_size + i+1;
                self->body_size = ((content_size+readded) - self->body_start_index);
                self->extra_body_remaning_to_send = self->body_size;
                self->body_readded = self->body_size;
                self->body = (unsigned char*)BearsslHttps_allocate(self->body_size+2);
                memcpy(self->body,self->raw_content + self->body_start_index,self->body_size);
                self->body[self->body_size] = '\0';

                private_BearHttpsResponse_parse_headers(self,content_size + i-1);
                return;
            }
        }
        content_size+=readded;

    }


    BearHttpsResponse_set_error(self,"invalid http response",BEARSSL_HTTPS_INVALID_HTTP_RESPONSE);


}
