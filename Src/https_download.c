#include "mbedtls.h"

#include "ctr_drbg.h"
#include "net_sockets.h"
#include "entropy.h"
#include "entropy_poll.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "https_download.h"
#include "Iot_config.h"
#include "W25QXX.h"
#include "md5.h"
#include "mqtt_connect.h"

extern     mbedtls_net_context server_fd;
extern     mbedtls_entropy_context entropy;
extern     mbedtls_ctr_drbg_context ctr_drbg;
extern     mbedtls_ssl_context ssl;
extern     mbedtls_ssl_config conf;
extern     mbedtls_ssl_session saved_session;
extern     mbedtls_x509_crt cacert;
extern 			App_Pram	 *App_Pram_data;
extern  uint8_t    NetOpen_Status_1 ;
uint32_t App_Write_Offset;
extern uint32_t   Lan_rev_lenth;
extern char       ServerIP[20];
extern uint8_t    Cline_Status_1;
extern uint8_t    SIM_Init_OK;
extern uint8_t   Wait_times ;            //超时时间
extern uint8_t 	Rev_flag;
extern char       Lan_rev_data[SIM_REV_BUF_MAX] ;
extern uint32_t   Lan_rev_lenth;
extern uint8_t  Start_upgr;

char  GETing_REQUEST[1024];
char ota_server_root_cert[]=
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\r\n"
"A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\r\n"
"b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\r\n"
"MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\r\n"
"YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\r\n"
"aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\r\n"
"jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\r\n"
"xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\r\n"
"1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\r\n"
"snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\r\n"
"U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\r\n"
"9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\r\n"
"BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\r\n"
"AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\r\n"
"yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\r\n"
"38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\r\n"
"AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\r\n"
"DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\r\n"
"HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\r\n"
"-----END CERTIFICATE-----\r\n";

static int tcpc_httpc_dnld_on_data(tcpc_httpc_t* httpc, uint16_t pkt_len, uint8_t* pkt_ptr)
{
	uint16_t i,m;
	uint8_t read_buf[512];
	uint32_t  lenth;
	uint32_t  pkt_offset;
	uint32_t  write_addr;
	int ret = (int)pkt_ptr;
	printf("Httpc:Content-Length:%lu\r\n", httpc->content_len);
//	httpc->content_len =0;
	if(0 == httpc->content_len){//try to find the whole head
		char* p;
		printf("try to find the whole head!\n\r");	
		if(NULL == httpc->phead)
		{
			printf("NULL == httpc->phead!\n\r");	
			p = strstr((char*)pkt_ptr, "\r\n\r\n");//找到头部结束位置
			if(p){
				printf("p = strstr((char*)pkt_ptr!\n\r");					
				httpc->phead = (uint8_t*)calloc(HTTP_HEAD_SIZE,sizeof(uint8_t));
				if(httpc->phead == NULL)
				{
					printf("httpc->phead = (uint8_t*)malloc ==NULL!\n\r");
					goto err_close_exit;
				}
					
				memcpy(httpc->phead, pkt_ptr, pkt_len);
				httpc->head_len = (uint32_t)p - (uint32_t)pkt_ptr;
			}

			if(0 != memcmp(pkt_ptr, "HTTP/1.1 200 OK", sizeof("HTTP/1.1 200 OK")-1) &&
			    0 != memcmp(pkt_ptr, "HTTP/1.0 200 OK", sizeof("HTTP/1.0 200 OK")-1)){
				pkt_ptr[pkt_len] = '\0';
				printf( "pkt_ptr[%d]:", pkt_len);	
				//goto err_close_exit;
			}
			
			printf("dnld:Resp 200 OK.\r\n");
			if(NULL == (p = strstr((char*)pkt_ptr, "Content-Length:")) ||
		  		0 == (httpc->content_len = atoi(p + sizeof("Content-Length:")))){
				printf("Content-Length err.");
				ret = -2;
				goto err_close_exit;
				}

			 printf("Httpc:Content-Length:%lu\r\n", httpc->content_len);

			p = strstr((char*)pkt_ptr, "\r\n\r\n");

			if(NULL == p){
				printf("Header err.\n");
				ret = -3;
				goto err_close_exit;
			}
			printf("Header OK.\n");
			p += 4;	

			pkt_len -= (p - (char*)pkt_ptr);
			pkt_ptr = (uint8_t*)p;
		}else{
			printf("Else .....try to find the whole head!\n\r");
			if((httpc->head_len + pkt_len) > HTTP_HEAD_SIZE){
				printf("dnld:size big than header.\r\n");//todo
				free(httpc->phead);
				httpc->phead = NULL;
				goto err_close_exit;
			}
			printf( "httpc->phead : %s \n", httpc->phead);
			printf( "httpc->head_len : %d \n", httpc->head_len);
			memcpy(httpc->phead + httpc->head_len, pkt_ptr, pkt_len);
			httpc->head_len += pkt_len;
			p = strstr((char*)httpc->phead, "\r\n\r\n");//找到头部结束位置
			if((char *)NULL == p){
				if(httpc->head_len >= HTTP_HEAD_SIZE){
					printf("dnld:Header not found.\r\n");
					free(httpc->phead);
					httpc->phead = NULL;
					goto err_close_exit;
				}
				return 0;
			}
			pkt_ptr = httpc->phead;
			pkt_len = httpc->head_len;
		}
	}
	if(0 == httpc->content_len){//must have found the head, and try to get content_len
		char* p;
			printf("must have found the head, and try to get content_len!\n\r");
		if(0 != memcmp(pkt_ptr, "HTTP/1.1 200 OK", sizeof("HTTP/1.1 200 OK")-1)){
			pkt_ptr[sizeof("HTTP/1.x 200 ")-1] = '\0';
			printf("HTTP resps:%s", (const char*)pkt_ptr);
			ret = -1;
			goto err_close_exit;
		}

		printf("Httpc:Resp 200 OK.\r\n");

		if(NULL == (p = strstr((char*)pkt_ptr, "Content-Length:")) ||
		   0 == (httpc->content_len = atoi(p + sizeof("Content-Length:")))){
			printf("Content-Length err.");
			ret = -2;
			goto err_close_exit;
		}
		printf("Httpc:Content-Length:%lu\r\n", httpc->content_len);
		//找到头部结束位置
		p = strstr((char*)pkt_ptr, "\r\n\r\n");
		if(NULL == p){
			printf("Header err.\n");
			ret = -3;
			goto err_close_exit;
		}
		printf("Header OK.\n");
		p += 4;			
		pkt_len -= (p - (char*)pkt_ptr);
		pkt_ptr = (uint8_t*)p;
	}
	pkt_len = MIN(pkt_len, httpc->content_len - httpc->body_processed);
	lenth =pkt_len;
	write_addr = Firmware_SPI_flash_addr+App_Write_Offset;
	printf("write_addr = 0x%08x \r\n",write_addr);
	printf("1: lenth %d,AppxWrite_Offset : 0x%02x\r\n",lenth,App_Write_Offset);
	
	//BSP_W25Qx_Read(read_buf,write_addr,pkt_len);
	//for(i=0;i<pkt_len;i++)
	//{
	//	printf("%d : 0x%02x : 0x%02x \r\n",i,read_buf[i],pkt_ptr[i]);
	//}
	pkt_offset =0;
	while(lenth>256)
	{
			BSP_W25Qx_Write( pkt_ptr+pkt_offset, Firmware_SPI_flash_addr+App_Write_Offset, 256);
			App_Write_Offset=App_Write_Offset+256;
			pkt_offset =pkt_offset +256;
		  lenth =lenth-256;
			printf("2:lenth %d,App_Write_Offset %d\r\n",lenth,App_Write_Offset);
	}
	printf("3:lenth %d\r\n",lenth);
	if(lenth>0)
	{
			BSP_W25Qx_Write( pkt_ptr+pkt_offset, Firmware_SPI_flash_addr+App_Write_Offset, lenth);
			App_Write_Offset =App_Write_Offset+lenth;
			pkt_offset =pkt_offset + lenth;
			lenth = 0;
	}
	//printf("4: pkt_len %d,App_Write_Offset %d\r\n",pkt_len,App_Write_Offset);
//	printf("write_addr = 0x%08x \r\n",write_addr);
	BSP_W25Qx_Read(read_buf,write_addr,pkt_len);
//	for(i=0;i<pkt_len;i++)
	//{
//		printf("%d : 0x%02x : 0x%02x \r\n",i,read_buf[i],pkt_ptr[i]);
//	}
	for(i=0;i<pkt_len;i++)
	{
	//	BSP_W25Qx_Read(&read_data,Firmware_SPI_flash_addr+App_Write_Offset-pkt_len+i,1);
		//printf("%d : 0x%02x : 0x%02x \r\n",i,read_buf[i],pkt_ptr[i]);
		if(read_buf[i]!=pkt_ptr[i])
		{
				printf("Data write Error!\n\r");
				Start_upgr =0;
				goto err_close_exit;
		}
	}

	printf("\n\r 5: write addr&check %08x OK!\n\r",App_Write_Offset);
	
	httpc->body_processed += pkt_len;
	if(httpc->body_processed >= httpc->content_len){
		printf("Httpc:Done(%ubytes).\r\n", (uint32_t)httpc->body_processed);
		ret = 0;
		printf("httpc->body_processed: %d >= httpc->content_len:%d\n",httpc->body_processed,httpc->content_len);	
		goto close_exit;
	}
	free(httpc->phead);
	return ret;

err_close_exit:
	ret=-1;
	printf("Httpc:err,exit.\r\n");

close_exit:
	free(httpc->phead);
	return ret;
}
void url_config(char *url,char *url_temp, char *serv_addr)
{
	int i,j,n;
 // char *url ="https://footfall.valueretailchina.com/ALL_AT_AliyunV120.bin";

	for(i=0;i<strlen(url);i++)
	{
		if((url[i]=='/')&&(url[i+1]=='/'))
		{
				for(j=i;j<strlen(url);j++)
				{
						if((url[j]=='/')&&(url[j+1]=='o')&&(url[j+2]=='t')&&(url[j+3]=='a')&&(url[j+4]=='/'))
						{
								for(n=0;n<(j-(i+2));n++)
								{
										serv_addr[n] =  url[i+2+n];
								}
								printf("serv_addr: %s \r\n",serv_addr);
								for(n=0;n<(strlen(url)-j);n++)
								{
									url_temp[n] =  url[j+n];
								}
								printf("url_temp: %s \r\n",url_temp);
								break;
						}
				}
		
		}
	}
}

uint8_t md5Check(char *Firmware_decrypt)
{
		uint32_t i;
		uint8_t read_data;
		mbedtls_md5_context md5_ctx;
    unsigned char decrypt[16];
		char decrypt_1[16];
		uint16_t len;
		
    
	  mbedtls_md5_init(&md5_ctx);
    mbedtls_md5_starts(&md5_ctx);
		printf("Read : \n\r"); 
		for(i=0;i<App_Pram_data->Firmware_size;i++)
		{
			BSP_W25Qx_Read(&read_data,Firmware_SPI_flash_addr+i,1);
			mbedtls_md5_update(&md5_ctx, &read_data, 1);
		}
    printf("\n\rRead end \n\r"); 
    mbedtls_md5_finish(&md5_ctx, decrypt);
		printf("decrypt md5 Check : \n\r"); 
		for (int i = 0; i<16; i++)
		{
			printf("decrypt[%d]:%02x \r\n",i,decrypt[i]);
		}
		printf("\r\n");
		
		printf("Firmware_decrypt  md5 Check : %s \n\r",Firmware_decrypt);
		
		len = HextoString(Firmware_decrypt,32,decrypt_1);

		printf("Firmware_decrypt md5 Check : \n\r"); 
		for (int i = 0; i<16; i++)
		{
			printf("decrypt_1[%d]: %02x \r\n",i,decrypt_1[i]);
		}
		printf("\r\n");
		
    printf("md5 Check : \n\r");  
    for (int i = 0; i<16; i++)
    {
				printf("decrypt[%d]:%02x /decrypt_1[%d]: %02x \r\n",i,decrypt[i],i,decrypt_1[i]);
				if(decrypt_1[i]!=decrypt[i])
				{
						mbedtls_md5_free(&md5_ctx); 
						return 0;
				}
    }
    printf("\n");
    mbedtls_md5_free(&md5_ctx); 
		return 1;
}

int Https_init(void)
{

	int ret = 0;
  int sizelen;
	const char *cert = ota_server_root_cert;
	char port_str[16];
	
	MX_MBEDTLS_Init();
	    /*
     * 0. Initialize the RNG and the session data
     */
  sizelen = strlen(App_Pram_data->Device_Name);
	
  printf("Seeding the random number generator....\n\r");
  
   if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,(const unsigned char *) App_Pram_data->Device_Name,sizelen ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret );
        goto exit;
    }
		
  printf("ctr_drbg_seed OK\n\r");
   /*
     * 1. Initialize certificates
     */
   printf("Loading the CA root certificate ...\n\r");
   
    ret = mbedtls_x509_crt_parse( &cacert, (const unsigned char *) cert, strlen(cert)+1 );
  
    if( ret < 0 ){
		printf( " failed  !  mbedtls_x509_crt_parse returned -0x%x\n", -ret );
    goto exit;
	}
    printf("crt_parse OK\n\r");
	
	 /*
     * 2. Start the connection
     */
	
	
//	const char * host =serv_addr;// ServerIP;//"103.46.128.43";
	uint16_t port = SERVER_PORT;
	snprintf(port_str, sizeof(port_str), "%d", port);
	printf(" tls: connect to server %s ,port is %s.\n\r",ServerIP, port_str);
	if(IOT_GSM)
	{
		if( (ret = mbedtls_net_connect( &server_fd, ServerIP, port_str, MBEDTLS_NET_PROTO_TCP) ) != 0){
//	if( (ret = mbedtls_net_connect( &server_fd, "103.46.128.43", "46389", MBEDTLS_NET_PROTO_TCP) ) != 0){
		printf( " failed ! mbedtls_net_connect returned -0x%x\r\n", -ret );
		goto exit;
		}
	}
	else
	{
		if( (ret = mbedtls_net_connect( &server_fd, SERVER_NAME, port_str, MBEDTLS_NET_PROTO_TCP) ) != 0){
//	if( (ret = mbedtls_net_connect( &server_fd, "103.46.128.43", "46389", MBEDTLS_NET_PROTO_TCP) ) != 0){
		printf( " failed ! mbedtls_net_connect returned -0x%x\r\n", -ret );
		goto exit;
		}	
	
	}

	    /*
     * 3. Setup conf
     */
		printf( "  . Setting up the SSL/TLS structure...a\n\r" );
   if( ( ret = mbedtls_ssl_config_defaults( &conf,
                    MBEDTLS_SSL_IS_CLIENT,
										MBEDTLS_SSL_TRANSPORT_STREAM,
                    MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
    {
    	printf( " failed ! mbedtls_ssl_config_defaults returned -0x%x\n\n", -ret );
			goto exit;
    }
    mbedtls_ssl_conf_authmode( &conf, MBEDTLS_SSL_VERIFY_OPTIONAL);//MBEDTLS_SSL_VERIFY_NONE  MBEDTLS_SSL_VERIFY_REQUIRED

  	mbedtls_ssl_conf_ca_chain( &conf, &cacert, NULL );

		mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );
 /*
	 * 4. ssl setup
	 */
		if( ( ret = mbedtls_ssl_setup( &ssl, &conf ) ) != 0 )
		{
			printf( " failed ! mbedtls_ssl_setup returned -0x%x\n\n", -ret );
      goto exit;
		}

		if( ( ret = mbedtls_ssl_set_hostname( &ssl, SERVER_NAME) ) != 0 )
		{
			printf( " failed ! mbedtls_ssl_set_hostname returned -0x%x\n\n", -ret );
			goto exit;
		}

		mbedtls_ssl_set_bio( &ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL );
  
		printf( " now, start handshake...\n\r" );
		if( ( ret = mbedtls_ssl_handshake(&ssl) ) != 0 )
    {
    	printf( " failed ! mbedtls_ssl_handshake returned -0x%x\n\n", -ret );
			goto exit;
    }
		printf("now, End mbedtls_ssl_handshake\n\r");
		printf( " ***tls handshake ok***\n\r    [ Protocol is %s ]\n\r    [ Ciphersuite is %s ]\n\r",	mbedtls_ssl_get_version( &ssl ), mbedtls_ssl_get_ciphersuite( &ssl ) );
		printf("now, start mbedtls_ssl_get_record_expansion\r");
    if( ( ret = mbedtls_ssl_get_record_expansion( &ssl ) ) >= 0 )
		printf( "    [ Record expansion is %d ]\n", ret );
		else
		{
			printf( "    [ Record expansion is unknown (compression) ]\n" );
			goto exit;
		}

		return 1;
exit:
    printf("Https_init mbedtls Error!!\n");
		Lan_rev_lenth =0;
		memset(Lan_rev_data,0,sizeof(Lan_rev_data));
		Rev_flag =0;
    mbedtls_net_free( &server_fd );
    mbedtls_x509_crt_free( &cacert );
    mbedtls_ssl_free( &ssl );
    mbedtls_ssl_config_free( &conf );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy ); 
		return 0;
}

int Https_download(uint32_t start_addr,uint32_t  end_addr)
{
	int ret = 0;
	char get_buf[30];
	char req_buf[512] ;
  uint8_t buf[2048];
	char url_temp[512];
	char serv_addr[512];
  uint16_t buf_len;
  uint32_t rec_len = 0;
  tcpc_httpc_t httpc;
  memset(&httpc, 0x00, sizeof(tcpc_httpc_t));
	char port_str[16];

	memset(serv_addr,0,512);
	memset(url_temp,0,512);
	url_config(App_Pram_data->Up_Server_addr,url_temp, serv_addr);

	snprintf(port_str, sizeof(port_str), "%d", SERVER_PORT);
		/*
	 * 6. write  Range:bytes=0-512
	 */
		printf("start_addr = %d ,end_addr = %d \n\r",start_addr,end_addr);
		sprintf(get_buf,"Range:bytes=%d-%d\r\n\r\n",start_addr,end_addr);
		sprintf(GETing_REQUEST,"GET %s HTTP/1.1\r\nhost: %s\r\nAccept: */*\r\n",url_temp,serv_addr);
		sprintf(req_buf,"%s%s",GETing_REQUEST,get_buf);
		printf( " now, writing...\n\r%s\r\n", req_buf );
		printf("start_addr = %d \n\r",start_addr);
		printf("tls: write  \n\r");

				printf("Start ssl write \r\n");
				Rev_flag=0;
				if( ( ret = mbedtls_ssl_write( &ssl, (const unsigned char *)req_buf, strlen(req_buf) ) ) < 0 )
				{
					printf("tls: write failed ,ret is -0x%x.\n\r",-ret);
					goto erroexit;
				}
		//		memset(&httpc, 0x00, sizeof(tcpc_httpc_t));	
				printf("mbedtls_ssl_read\r\n");

				ret = mbedtls_ssl_read( &ssl, buf, sizeof(buf));
				if(ret>0)
				{
					printf("ret > 0\r\n");
					printf("ret = %d\r\n",ret);
				}
				else
				{
					if( ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE ){
					buf_len = 0;
					goto erroexit;
						}
					else{
						if( ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY )
							printf("tls: peer close notify.\n\r");
						else if(ret == MBEDTLS_ERR_NET_CONN_RESET )
								printf("tls: peer reset.\n\r");
						buf_len = 0;
		        goto erroexit;
						}
				}

			printf("trying to read, ret: %d\r\n", ret);
                
			if(ret < 0){
					printf("ret < 0\r\n");
					if( ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE ){
					buf_len = 0;
					goto erroexit;
				}
			else{
				//对方断开的连接
				printf("对方断开的连接\r\n");
				if( ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY )
					printf("tls: peer close notify.\n\r");
				else if(ret == MBEDTLS_ERR_NET_CONN_RESET )
					printf("tls: peer reset.\n\r");
				buf_len = 0;
				goto erroexit;
			}
		}
		else{//recv data
			buf[ret] = '\0';
			buf_len = ret;
			printf("tcpc_httpc_dnld_on_data start!\r\n");
			ret = tcpc_httpc_dnld_on_data(&httpc, buf_len, buf);
			if(ret == 0){
				goto done;
			}else if(ret < 0){
				goto erroexit;
			}
		}
	printf("start_addr = %d App_Pram_data->Firmware_size = %d \n\r",start_addr,App_Pram_data->Firmware_size);
done:
		Lan_rev_lenth =0;
		memset(Lan_rev_data,0,sizeof(Lan_rev_data));
		Rev_flag =0;
    mbedtls_net_free( &server_fd );
    mbedtls_x509_crt_free( &cacert );
    mbedtls_ssl_free( &ssl );
    mbedtls_ssl_config_free( &conf );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );
		return 1;
erroexit:
    printf(" mbedtls Error!!\n");
		Lan_rev_lenth =0;
		memset(Lan_rev_data,0,sizeof(Lan_rev_data));
		Rev_flag =0;
    mbedtls_net_free( &server_fd );
    mbedtls_x509_crt_free( &cacert );
    mbedtls_ssl_free( &ssl );
    mbedtls_ssl_config_free( &conf );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy ); 
		return 0;
}

