# ipc_window
inter process communication in window memory  

window server, client inter communication use window memory  
memory mapping with name,   
make read, write thread  

communication with simple protocol  
if someone use this memory, cehck first byte buf[0] is 'u'  
or not, 'f' check  

and second byte buf[1] is write who  
's' is server  
'c' is client  

third byte buf[2] is length  
string length is here  

buf[3] ~ 
message  

