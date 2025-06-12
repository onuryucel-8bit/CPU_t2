#include <iostream>
#include <vector>
#include <cmath>

enum ControlBitNo
{
    //Sol Kontrol Bitleri 
    RAM_out,
    RAM_we,
    PC_enb,
    PC_load,
    MUX_enb,

    MAR_we,
    MDR_we,
    MDR_out,

    //Sag Kontrol Bitleri 
    I3B,
    ACC_we,
    ALUMUX_enb,
    ALU_out,
    
    TEMP_we,
    //*******************
    IR_we,
    REGPATH_in,
    REGFILE_we,
    REGFILE_out,

    CMP_out,
    OUT_we,

    TEMP_out,//TEMP => data bus
    
    //Kontrol Birimi
    postaA_out,//19
    postaA_we,//20
    
    postaB_out,//21
    postaB_we,//22

    adrReg_out,//23
    adrReg_we, //24

};

//a00080
std::vector<int> inputArray =
{
    
    MDR_out,
	postaA_we,
	postaB_we,
 
};

void calcControlBits()
{
    
    int res = 0;
    for (size_t i = 0; i < inputArray.size(); i++)
    {
        res += std::pow(2, inputArray[i]);
    }
    std::cout << "hex : " << std::hex << res << "\n";
    //std::cout << std::dec;
    
}


int main()
{
    std::cout << "--------------\n";

    calcControlBits();

}
