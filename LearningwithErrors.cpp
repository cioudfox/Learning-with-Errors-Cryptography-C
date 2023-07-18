#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <random>

//----------------------------------------------------------------------------------
class func_enc {

    public:
        const int mod_val = 7919;
        const int alph_size = 128;
        const int alph_dist = (int)(mod_val / 128);
        void publickey();
        void encrypt();
        void decrypt();
        std::vector<int> getcombinedequation();
        std::vector<int> privkeygen();

    protected:
        std::vector<int> privkey;
};
//----------------------------------------------------------------------------------
// Description:
//   --Function generates a Public Key Text File using given a private key
void func_enc :: publickey()
{
    //Generate Public Key
    double offset; 
    privkey = privkeygen();
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    std::ofstream filekey("Public_Key.txt");
    
    std::vector<std::vector<int>> A(1000, std::vector<int>(5));
    for(int i=0; i<1000; i++)
    {
        for(int j=0; j<5; j++)
        {
            A[i][j] = dis(gen);
            std::mt19937 gen(rd());
            offset = (((dis(gen) * 29) + 177) % 4);
            if(j == 4)
            {
                A[i][4] = (int)(( (A[i][0] * privkey[0]) + (A[i][1] * privkey[1]) + (A[i][2] * privkey[2]) 
                          + (A[i][3] * privkey[3])) + offset) % mod_val;
            }
        }
        std::ostringstream sstr1;
        sstr1  << std::setw(10) << std::setfill('0') << std::right << std::internal << A[i][0]
        << " " << std::setw(10) << std::setfill('0') << std::right << std::internal << A[i][1]
        << " " << std::setw(10) << std::setfill('0') << std::right << std::internal << A[i][2]
        << " " << std::setw(10) << std::setfill('0') << std::right << std::internal << A[i][3]
        << " " << std::setw(10) << std::setfill('0') << std::right << std::internal << A[i][4]
        << " \n";

        if (filekey.is_open()) {
            filekey << sstr1.str();
        }
    }
    std::cout << "Public Key Generated" << std::endl << std::endl;
    filekey.close();

}


//----------------------------------------------------------------------------------
// Encrypt()
//    -- Takes a Ascii Message and Encodes using the Public Key
//    -- 
void func_enc :: encrypt()
{
    std::ofstream filekey("Encoded_Key.txt");
    std::string msg = "";
    std::ostringstream oss;
    char asclett;
    int ind_num;

    std::cout << "Enter Message to Encrypt" << std::endl;
    getline(std::cin, msg);
    

    for(int i = 0; i < msg.length(); i++)
    {
        asclett = msg[i];
        ind_num = (int)asclett;
        std::vector<int> combeq = getcombinedequation();

        //Solve for Summation with index distance added in.
        int tempval;        
        tempval = ((ind_num * alph_dist) + combeq[4]) % mod_val;
        combeq[4] = tempval;

        //Format Calculated values to be stored into file
        oss << std::setw(10) << std::setfill('0') << std::right << std::internal << combeq[0]
        << " " << std::setw(10) << std::setfill('0') << std::right << std::internal << combeq[1]
        << " " << std::setw(10) << std::setfill('0') << std::right << std::internal << combeq[2]
        << " " << std::setw(10) << std::setfill('0') << std::right << std::internal << combeq[3]
        << " " << std::setw(10) << std::setfill('0') << std::right << std::internal << combeq[4]
        << " \n";
        if (filekey.is_open()) 
        {
            filekey << oss.str();
        }
        std::ostringstream().swap(oss);
    }
    std::cout << "Encoded Message Complete" << std::endl << std::endl;
    filekey.close();
}
//----------------------------------------------------------------------------------
void func_enc :: decrypt()
{
    std::ifstream filekey("Encoded_Key.txt");
    std::ofstream filekey2("Decoded_Key.txt");

    std::string str;
    std::stringstream ss;
    std::vector<int> privatekey;
    int val;

    std::cout << "Enter Private Key"<< std::endl;
    getline(std::cin, str);
    ss << str;
    if(str == "" || str.length() < 5)
    {
        std::cout << "Invalid Entry" << std::endl;
        return;
    }
    while(ss >> val)
    {
        privatekey.push_back(val);
    }
    std::stringstream().swap(ss);
 
    while (getline(filekey, str)) {
        std::vector<int> equation;    
        float tempval;
        int estsum;
        
        // do something with the line
        ss << str;
        while(ss >> val)
        {
            equation.push_back(val);
        }
        tempval = ((equation[0] * privatekey[0]) + (equation[1] * privatekey[1]) + (equation[2] * privatekey[2])
             + (equation[3] * privatekey[3])) % mod_val;

        estsum = (equation[4] - tempval);

        //If negative, find its inverse
        if(estsum < 0)
        {
            estsum += mod_val;
        }

        estsum = (int)estsum/alph_dist;
        std::stringstream().swap(ss);

        if (filekey2.is_open()) {
            filekey2 << char(estsum);
        }
    }
    std::cout << "Decoded Message Complete" << std::endl << std::endl;
    filekey.close();
    filekey2.close();
}
//----------------------------------------------------------------------------------
std::vector<int> func_enc :: privkeygen()
{
    std::string str;
    std::cout << "Enter input val(4 integers separated by spaces)"<< std::endl;
    getline(std::cin, str);
    
    std::stringstream ss(str);
    int x;
    while(ss >> x)
    {
        privkey.push_back(x);
    }
    std::cout << "Private Key Generated " << std::endl << std::endl;
    return privkey;
}

//----------------------------------------------------------------------------------
std::vector<int> func_enc :: getcombinedequation()
{
    //Select a selection of 4 values
    //Modulus 
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);

    std::ifstream Public_Key_File("Public_Key.txt");

    //Defined Variables
    std::string str;
    int x;

    std::vector<int> public_key_index;
    std::vector<int> combined_equation = {0, 0, 0, 0, 0};
    std::stringstream ss;

    public_key_index = {dis(gen),dis(gen),dis(gen),dis(gen)};
    std::cout << "Auto-Selection of Indexes: " << public_key_index[0] << "  " << public_key_index[1] << "  "  << 
                        public_key_index[2] << "  " << public_key_index[3] << std::endl;

    //Bubble sort the Selection of Equations
    for(int i = 0; i < public_key_index.size();i++)
    {
      for(int j = i + 1; j < public_key_index.size(); ++j)
      {
         if(public_key_index[i] > public_key_index[j])
         {
            int temp = public_key_index[i];
            public_key_index[i] = public_key_index[j];
            public_key_index[j] = temp;
         }
      }
    }

    //Generate the Combined equation from Public Key
    for(int i = 0; i < public_key_index.size(); i++)
    {
        int num = public_key_index[i];
        if(i > 0)
            num -= public_key_index[i-1] + 1;
        for(int j = 0; j < num + 1; j++)
        {
            std::getline(Public_Key_File, str);
        }

        ss << str;
        std::vector<int> temp_equation;
        while(ss >> x)
        {
            temp_equation.push_back(x);
        }
        
        for(int k = 0; k < 5; k++)
        {
            combined_equation[k] += temp_equation[k];
            if(k == 4)
            {
                combined_equation[k] = combined_equation[k] % mod_val;
            }
        }
        std::stringstream().swap(ss);
    }

    Public_Key_File.close();
    return combined_equation;
}

//----------------------------------------------------------------------------------
int main()
{
    func_enc beat;
    bool flag = false;
    int val;
    while(true) {
        std::cout << std::endl << "Make selection\n1 --Generate Public Key\n2 --" <<  
                    "Encrypt Message\n3 --Decrypt Message\n-1 --Exit\n";
        std::cin >> val;
        std::cin.ignore(256, '\n'); 
        switch (val) {
            case 1:
                beat.publickey();
                break;
            case 2:
                beat.encrypt();
                break;
            case 3:
                beat.decrypt();
                break;
            case -1:
                flag = true;
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
        if(flag) break;
    }
}