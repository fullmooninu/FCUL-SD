


bool isValidOPC(short opcode){
  return (opcode != NULL || opcode == OC_SIZE || opcode == OC_DEL || opcode == OC_UPDATE
    || opcode == OP_GET || opcode == OC_PUT);
}
bool isValidCTC(short c_type){
  return (c_type != NULL || c_type == CT_RESULT || c_type == CT_VALUE || c_type == CT_KEY
    || c_type == CT_KEYS || c_type == CT_ENTRY);
}
