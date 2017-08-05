#include "dynamixel.h"

Dynamixel::Dynamixel(QObject *parent) : QObject(parent)
{

}
void Dynamixel::Init(QString portName,int baudRate)
{
    _media.Init(portName,baudRate,2);
}
void Dynamixel::SetLed(unsigned char id, bool status)
{
//    Instruction_Packet_Array[0] = ID;
//    Instruction_Packet_Array[1] = LED_LENGTH;
//    Instruction_Packet_Array[2] = COMMAND_WRITE_DATA;
//    Instruction_Packet_Array[3] = RAM_LED;
//    Instruction_Packet_Array[4] = Status;
    QByteArray packet;
    packet.push_back(id);
    packet.push_back(LED_LENGTH);
    packet.push_back(COMMAND_WRITE_DATA);
    packet.push_back(RAM_LED);
    packet.push_back(status);
    SendCommand(packet);


}
unsigned int Dynamixel::SetPosition(unsigned char id,unsigned int position,unsigned int speed){
    QByteArray packet;
    packet.append(id);
    packet.append(SERVO_GOAL_LENGTH);
    packet.append(COMMAND_WRITE_DATA);
    packet.append(RAM_GOAL_POSITION_L);
    packet.append(position&0xff);
    packet.append(position>>8);
    packet.append(speed&0xff);
    packet.append(speed>>8);
    SendCommand(packet);
//    Instruction_Packet_Array[0] = ID;
//    Instruction_Packet_Array[1] = SERVO_GOAL_LENGTH;
//    Instruction_Packet_Array[2] = COMMAND_WRITE_DATA;
//    Instruction_Packet_Array[3] = RAM_GOAL_POSITION_L;
//    Instruction_Packet_Array[4] = byte(Position);
//    Instruction_Packet_Array[5] = byte((Position & 0x0F00) >> 8);
//    Instruction_Packet_Array[6] = byte(Speed);
//    Instruction_Packet_Array[7] = byte((Speed & 0x0F00) >> 8);

//    clearRXbuffer();

//    transmitInstructionPacket();


//    if (ID                                            == 0XFE || Status_Return_Value != ALL ){     // If ID of FE is used no status packets are returned so we do not need to check it
//        return (0x00);
//    }else{
//        readStatusPacket();
//        if (Status_Packet_Array[2] == 0){               // If there is no status packet error return value
//            return (Status_Packet_Array[0]);            // Return SERVO ID
//        }else{
//            return (Status_Packet_Array[2] | 0xF000);   // If there is a error Returns error value
//        }
//    }

}
unsigned int Dynamixel::ReadPosition(unsigned char id){
    QByteArray packet,replay;
    packet.push_back(id);
    packet.push_back(READ_POS_LENGTH);
    packet.push_back(COMMAND_READ_DATA);
    packet.push_back(RAM_PRESENT_POSITION_L);
    packet.push_back(READ_TWO_BYTE_LENGTH);
    replay=SendCommand(packet);
if(replay.length()<3)return 0;
    if((int)(replay[2])!=id || replay.length()<7){

        return 0;
    }
    return replay[6]<<8+replay[5];

}
unsigned int Dynamixel::ReadVoltage(int id){
    QByteArray packet,replay;
    packet.push_back(id);
    packet.push_back(READ_VOLT_LENGTH);
    packet.push_back(COMMAND_READ_DATA);
    packet.push_back(RAM_PRESENT_VOLTAGE);
    packet.push_back(READ_ONE_BYTE_LENGTH);
    replay=SendCommand(packet);
if(replay.length()<3)return 0;
if((int)(replay[2])!=id || replay.length()<6){

    return 0;
}


return replay[5];
}
unsigned int Dynamixel::ReadTemperature(int id){
    QByteArray packet,replay;
    packet.push_back(id);
    packet.push_back(READ_TEMP_LENGTH);
    packet.push_back(COMMAND_READ_DATA);
    packet.push_back(RAM_PRESENT_TEMPERATURE);
    packet.push_back(READ_ONE_BYTE_LENGTH);
    replay=SendCommand(packet);
    if(replay.length()<3)return 0;
    if((int)(replay[2])!=id || replay.length()<6){

        return 0;
    }
return replay[5];

}
unsigned int Dynamixel::Ping(int id){

    QByteArray packet,replay;
    packet.push_back(id);
    packet.push_back(PING_LENGTH);
    packet.push_back(COMMAND_PING);
    replay=SendCommand(packet);
    if(replay.length()<3)return 0;
    if((int)(replay[2])!=id || replay.length()<6){

        return 0;
    }
    if(replay[4]!=0)//check error
   return 0;
    return replay[2];



}
QByteArray Dynamixel::SendCommand(QByteArray command)
{
 QByteArray packet;
 packet.push_back(HEADER);
 packet.push_back(HEADER);
 packet.push_back(command[0]);
 packet.push_back(command[1]);
 packet.push_back(command[2]);
 unsigned int checksumPacket = command[0] + command[1] + command[2];
 for (unsigned char i = 3; i <= command[1]; i++){
      packet.push_back(command[i]);                                   // Write Instuction & Parameters (if there are any) to serial
      checksumPacket += command[i];
  }
  packet.push_back(~checksumPacket & 0xFF);
  QByteArray replay;
  _media.WriteCommand(packet,replay,6,6);
return replay;


}


