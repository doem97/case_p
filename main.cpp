#include "allmyfile.h"

//#define LRU2_BYTE_CNT_WIDTH 65535  //16, 2^16-1
//#define LRU2_PKT_CNT_WIDTH 65535 //6:63,7:127,8:255,9:511,10:1023,11:2047,12:4095,13:8191,14:16383,15:32767,16:65535
#define SUPP_FLOW   2097152*10//2M, 2*1024*1024

// define the accesss time of the chip
//#define OFF_CHIP_TIME 2.37 //IMPORTANT: Time here counts
// SigmaQuad SRAM IVe
// cascade two SRAM 36-bit width: 2.37ns(633MHz), 1.8ns(833MHz), 1.125ns(1333MHz)
//#define ON_CHIP_TIME  2//CACHE 2ns or 633MHz 1.58ns
//#define CALC_TIME     5// parallel 4 calculation modules
#define BURST_DRAM_TIME 5//RLDRAM II 20ns, burst length = 4;

int main(int argc,char*argv[])
{
	bool use_command = false;
    bool use_pkt_th = true; //true to use packet threshold, and false to use byte threshold.
    int THRES_PKT;
    int THRES_BYTE;
    double size_1_2_rate;
    int CACHE_SIZE_TOTAL;
    int CACHE_SIZE_1; // 16,32,48,64,80,96,112
    int CACHE_SIZE_2;
    int LRU2_PKT_CNT_WIDTH;
    int LRU2_BYTE_CNT_WIDTH;
    double OFF_CHIP_TIME;
    double ON_CHIP_TIME;
    double CALC_TIME;
    std::string base_folder = "D:\\workspace\\CASE_P\\";
    std::string temp_string;
    FILE * fp;
	if(use_command == true)
	{
	    THRES_PKT = atoi(argv[1]);
	    THRES_BYTE = atoi(argv[2]);
	    size_1_2_rate = atof(argv[3]);
	    CACHE_SIZE_TOTAL = 1024 * atof(argv[4]);
        CACHE_SIZE_1 = CACHE_SIZE_TOTAL * size_1_2_rate;
	    CACHE_SIZE_2 = CACHE_SIZE_TOTAL - CACHE_SIZE_1;
        LRU2_PKT_CNT_WIDTH = atoi(argv[5]);
        LRU2_BYTE_CNT_WIDTH = atoi(argv[6]);
        ON_CHIP_TIME = atof(argv[7]);
        OFF_CHIP_TIME = atof(argv[8]);
        CALC_TIME = atof(argv[9]);
        temp_string = base_folder + "data\\trace\\" + argv[10];
		fp = fopen(temp_string.c_str(), "r");
	}
	else
	{
	    THRES_PKT = 9;
	    THRES_BYTE = 8192;
	    size_1_2_rate = 0.125;
	    CACHE_SIZE_TOTAL = 1024 * 128;
<<<<<<< HEAD
	    CACHE_SIZE_1 = CACHE_SIZE_TOTAL * size_1_2_rate;
	    CACHE_SIZE_2 = CACHE_SIZE_TOTAL - CACHE_SIZE_1;
		fp = fopen("data\\trace\\report_2013.txt","r");
=======
        CACHE_SIZE_1 = 128;
	    CACHE_SIZE_2 = 1024;
        LRU2_PKT_CNT_WIDTH = 1023;
        LRU2_BYTE_CNT_WIDTH = 524288;
        //ON_CHIP_TIME = 1.58;
        ON_CHIP_TIME = 5;
        OFF_CHIP_TIME = 2.37;
        CALC_TIME = 20;
        temp_string = base_folder + "data\\trace\\" + "report_2013.txt";
		fp = fopen(temp_string.c_str(), "r");
>>>>>>> lru2logic
	}

	printf("%d pkt_th, %d byte_th, %f cut_rate, %d LRU_1, %d LRU_2, %d LRU2_pkt_width, %d LRU2_byte_width, %f on_chip_time, %f off_chip_time, %f calc_time\n", THRES_PKT, THRES_BYTE, size_1_2_rate, CACHE_SIZE_1, CACHE_SIZE_2, LRU2_PKT_CNT_WIDTH, LRU2_BYTE_CNT_WIDTH, ON_CHIP_TIME, OFF_CHIP_TIME, CALC_TIME);

    double scale_byte = pow(0.5, 11.730474);
    //scale parameters, width = 16;
    double scale_byte_11 = pow(0.5, 11.730474);//scale 3
    double scale_byte_12 = pow(0.5, 12.0);//scale 2
    double scale_byte_15 = pow(0.5, 15.0);//scale 1
    double scale_pkt = pow(0.5,8.0551);
    //scale parameters, width = 16;
    double scale_pkt_8 = pow(0.5, 8.0551);//scale 3
    double scale_pkt_10 = pow(0.5, 10);//scale 2
    double scale_pkt_11 = pow(0.5, 11);//scale 1
    //scale 0
    static int scale_value_byte[SUPP_FLOW];
    static int scale_value_pkt[SUPP_FLOW];

    static long real_value_byte[SUPP_FLOW];
    static long real_value_pkt[SUPP_FLOW];
    static long symb_value_byte[SUPP_FLOW];
    static long symb_value_pkt[SUPP_FLOW];
    static long u_symb_value_byte[SUPP_FLOW];
    static long u_symb_value_pkt[SUPP_FLOW];
    //evict items from LRU_1
    static long pile_value_byte[SUPP_FLOW];
    static long pile_value_pkt[SUPP_FLOW];

    int i;
    for(i=0; i<SUPP_FLOW; i++)
    {
        scale_value_byte[i] = 0;
        scale_value_pkt[i] = 0;
        real_value_byte[i] = 0;
        real_value_pkt[i] = 0;
        symb_value_byte[i] = 0;
        symb_value_pkt[i] = 0;
        u_symb_value_byte[i] = 0;
        u_symb_value_pkt[i] = 0;
        pile_value_byte[i] = 0;
        pile_value_pkt[i] = 0;
    }

    dl_list_node * cache_node;

    // initialize LRU_1 double linked list;
    dl_list_head   list_head_1;
    dl_list_head * list_head_1_p;
    list_head_1_p = &list_head_1;

    list_head_1.head = NULL;
    list_head_1.tail = NULL;
    list_head_1.list_length = 0;

    // initialize the LRU_2;
    dl_list_head   list_head_2;
    dl_list_head * list_head_2_p;
    list_head_2_p = &list_head_2;

    list_head_2.head = NULL;
    list_head_2.tail = NULL;
    list_head_2.list_length = 0;

    if(fp == NULL)
    {
        printf("Failed to open the file.");
        return -1;
    }
    int Flow_ID_temp = 0;
    int ByteCnt_temp = 0;
    int PktCnt_temp = 0;

    int cache_hit = 0;
    int cache_miss = 0;
    float cache_hit_rate = 0;
    int overflow_time_pkt = 0;
    int overflow_time_byte = 0;

    int total_flow_cnt = 0;  //trace all flow count;
    double total_byte_cnt = 0;
    int total_pkt_cnt = 0;   //trace all packet count;
    int upscale_cnt_byte = 0;
    int upscale_cnt_pkt = 0;

    int temp_key = 0;

    int LRU1_byte_counter_max = 0;
    int LRU1_pkt_counter_max = 0;
    int sram_trans = 0;
    int dram_trans = 0;

    /*throughput*/
    double access_time = 0;//can be used in this trace

    //off-chip DRAM, debug, 20180129
    int dram_consumption = 0; //dram: once off-chip write, it will take one entry in dram.
    int sram_consumption = 0; //difference from dram, sram uses update algorithm, so only a new flow costs a new entry.

    while(fscanf(fp, "%d %d", &Flow_ID_temp, &ByteCnt_temp) != EOF)
    {
        //for pkt counting
        PktCnt_temp = 1;

        if(Flow_ID_temp > total_flow_cnt)  //record the total flow count
        {
            total_flow_cnt = Flow_ID_temp;
        }
        total_pkt_cnt ++;                  //record the total packet count
        total_byte_cnt += ByteCnt_temp;

        //************** record the real value*********************
        real_value_byte[Flow_ID_temp] += (long)ByteCnt_temp;
        real_value_pkt[Flow_ID_temp] += (long)PktCnt_temp;
        //************** record the uncached symbol value**********
        u_symb_value_byte[Flow_ID_temp] += OEFUpdate(u_symb_value_byte[Flow_ID_temp], (long)ByteCnt_temp, scale_byte);
        u_symb_value_pkt[Flow_ID_temp] += OEFUpdate(u_symb_value_pkt[Flow_ID_temp], (long)PktCnt_temp, scale_pkt);

        packet * found_item;
        found_item = find_user(Flow_ID_temp);
        //access_time += ON_CHIP_TIME; // time of searching two LRU

        if(found_item == NULL)//cache miss
        {
            cache_node = new dl_list_node;
            init_dl_list_node(cache_node, Flow_ID_temp, ByteCnt_temp, PktCnt_temp, list_head_1_p);
            if(list_head_1.list_length == CACHE_SIZE_1)
            {
                insert_node_head(list_head_1_p, cache_node);
                add_user(Flow_ID_temp, cache_node);

                ByteCnt_temp = list_head_1.tail->cached_value;
                PktCnt_temp = list_head_1.tail->cached_pkt;
                temp_key = delete_node_tail(list_head_1_p);
                packet * temp_packet;
                temp_packet = find_user(temp_key);
                delete_user(temp_packet);

                //add the value of evicted LRU_1 item to the array;
                pile_value_byte[temp_key] += ByteCnt_temp;
                pile_value_pkt[temp_key] += PktCnt_temp;
                //20180205 debug
                dram_consumption ++;
                dram_trans += 1;

                /*throughput*/
                //only one write and one read of on-chip cache and one burst write of off-chip RLDRAM II;
                access_time += ON_CHIP_TIME*2;

            }
            else
            {
                insert_node_head(list_head_1_p, cache_node);
                add_user(Flow_ID_temp, cache_node);

                /*throughput*/
                //only one write of on-chip cache;
                access_time += ON_CHIP_TIME;
            }
            cache_miss ++;
        }
        else // cache hit
        {
            if(found_item->hash_to_list->list_head_p == list_head_1_p)//LRU_1 hit
            {
                if((found_item -> hash_to_list -> cached_value + ByteCnt_temp >= THRES_BYTE && !use_pkt_th) || (found_item -> hash_to_list -> cached_pkt + PktCnt_temp >= THRES_PKT && use_pkt_th))//from LRU_1 to LRU_2
                {
                    //delete item from LRU_1
                    ByteCnt_temp = found_item->hash_to_list->cached_value + ByteCnt_temp;
                    if(found_item->hash_to_list->cached_value > LRU1_byte_counter_max)
                        LRU1_byte_counter_max = found_item->hash_to_list->cached_value;
                    if(found_item->hash_to_list->cached_pkt > LRU1_pkt_counter_max)
                        LRU1_pkt_counter_max = found_item->hash_to_list->cached_pkt;
                    PktCnt_temp = found_item->hash_to_list->cached_pkt + PktCnt_temp;
                    temp_key = found_item->hash_to_list->cached_key;
                    delete_node_pointed(list_head_1_p, found_item->hash_to_list);
                    packet * temp_packet;
                    temp_packet = find_user(temp_key);
                    delete_user(temp_packet);

                    //add item to LRU_2
                    cache_node = new dl_list_node;
                    init_dl_list_node(cache_node, temp_key, ByteCnt_temp, PktCnt_temp, list_head_2_p);
                    if(list_head_2.list_length == CACHE_SIZE_2) // LRU_2 is full
                    {
                        insert_node_head(list_head_2_p, cache_node);
                        add_user(temp_key, cache_node);

                        ByteCnt_temp = list_head_2.tail->cached_value;
                        PktCnt_temp = list_head_2.tail->cached_pkt;
                        temp_key = delete_node_tail(list_head_2_p);
                        packet * temp_packet;
                        temp_packet = find_user(temp_key);
                        delete_user(temp_packet);

                        //overflow to SRAM
                        if(symb_value_byte[temp_key] == 0)
                                sram_consumption ++;

                        if(scale_value_byte[temp_key] == 0)
                        {
                            symb_value_byte[temp_key] += (long)ByteCnt_temp;
                            if(symb_value_byte[temp_key] > 65535)
                            {
                                scale_value_byte[temp_key] = 1;
                                symb_value_byte[temp_key]  = OEFUpdate(0, symb_value_byte[temp_key], scale_byte_15);
                                upscale_cnt_byte++;
                            }
                        }
                        else if(scale_value_byte[temp_key] == 1)//scale_byte_15
                        {
                            symb_value_byte[temp_key] += OEFUpdate(symb_value_byte[temp_key], (long)ByteCnt_temp, scale_byte_15);
                            if(symb_value_byte[temp_key] > 209345)
                            {
                                scale_value_byte[temp_key] = 2;
                                symb_value_byte[temp_key] = OEF_renor(scale_byte_15, scale_byte_12, symb_value_byte[temp_key]);
                                upscale_cnt_byte++;
                            }
                        }
                        else if(scale_value_byte[temp_key] == 2)//scale_byte_12
                        {
                            symb_value_byte[temp_key] += OEFUpdate(symb_value_byte[temp_key], (long)ByteCnt_temp, scale_byte_12);
                            if(symb_value_byte[temp_key] > 36322063329)
                            {
                                scale_value_byte[temp_key] = 3;
                                symb_value_byte[temp_key] = OEF_renor(scale_byte_12, scale_byte_11, symb_value_byte[temp_key]);
                                upscale_cnt_byte++;
                            }
                        }
                        else if(scale_value_byte[temp_key] == 3)//scale_byte_11
                        {
                            symb_value_byte[temp_key] += OEFUpdate(symb_value_byte[temp_key], (long)ByteCnt_temp, scale_byte_11);
                        }

                        // packet upscale
                        if(scale_value_pkt[temp_key] == 0)
                        {
                            symb_value_pkt[temp_key] += (long)PktCnt_temp;
                            if(symb_value_pkt[temp_key] > 4095)
                            {
                                scale_value_pkt[temp_key] = 1;
                                symb_value_pkt[temp_key]  = OEFUpdate(0, symb_value_pkt[temp_key], scale_pkt_11);
                                upscale_cnt_pkt++;
                            }
                        }
                        else if(scale_value_pkt[temp_key] == 1)//scale_pkt_11
                        {
                            symb_value_pkt[temp_key] += OEFUpdate(symb_value_pkt[temp_key], (long)PktCnt_temp, scale_pkt_11);
                            if(symb_value_pkt[temp_key] > 13073)
                            {
                                scale_value_pkt[temp_key] = 2;
                                symb_value_pkt[temp_key] = OEF_renor(scale_pkt_11, scale_pkt_10, symb_value_pkt[temp_key]);
                                upscale_cnt_pkt++;
                            }
                        }
                        else if(scale_value_pkt[temp_key] == 2)//scale_pkt_10
                        {
                            symb_value_pkt[temp_key] += OEFUpdate(symb_value_pkt[temp_key], (long)PktCnt_temp, scale_pkt_10);
                            if(symb_value_pkt[temp_key] > 54747)
                            {
                                scale_value_pkt[temp_key] = 3;
                                symb_value_pkt[temp_key] = OEF_renor(scale_pkt_10, scale_pkt_8, symb_value_pkt[temp_key]);
                                upscale_cnt_pkt++;
                            }
                        }
                        else if(scale_value_pkt[temp_key] == 3)//scale_pkt_8
                        {
                            symb_value_pkt[temp_key] += OEFUpdate(symb_value_pkt[temp_key], (long)PktCnt_temp, scale_pkt_8);
                        }

                        /*throughput*/
                        //only one write and two read of on-chip cache -> then update -> one read and one write to off-chip SRAM;
                        access_time += ON_CHIP_TIME*3 + OFF_CHIP_TIME + CALC_TIME;
                        sram_trans += 2;

                    }
                    else //LRU_2 not full
                    {
                        insert_node_head(list_head_2_p, cache_node);
                        add_user(Flow_ID_temp, cache_node);

                        /*throughput*/
                        //only one write and one read of on-chip cache;
                        access_time += ON_CHIP_TIME*2;
                    }

                }
                else //hit in LRU_1 without go to LRU_2
                {
                    //delete item from LRU_1
                    ByteCnt_temp = found_item->hash_to_list->cached_value + ByteCnt_temp;
                    PktCnt_temp = found_item->hash_to_list->cached_pkt + PktCnt_temp;
                    temp_key = found_item->hash_to_list->cached_key;
                    delete_node_pointed(list_head_1_p, found_item->hash_to_list);
                    packet * temp_packet;
                    temp_packet = find_user(temp_key);
                    delete_user(temp_packet);

                    //add item to LRU_1 in the head
                    cache_node = new dl_list_node;
                    init_dl_list_node(cache_node, temp_key, ByteCnt_temp, PktCnt_temp, list_head_1_p);
                    insert_node_head(list_head_1_p, cache_node);
                    add_user(temp_key, cache_node);

                    //only one read and one write of on-chip cache;
                    access_time += ON_CHIP_TIME*2;

                }
            }
            else if(found_item->hash_to_list->list_head_p == list_head_2_p)//LRU_2 hit
            {
                ByteCnt_temp = found_item->hash_to_list->cached_value + ByteCnt_temp;
                PktCnt_temp = found_item->hash_to_list->cached_pkt + PktCnt_temp;
                if(ByteCnt_temp > LRU2_BYTE_CNT_WIDTH || PktCnt_temp > LRU2_PKT_CNT_WIDTH)
                {
                    if(ByteCnt_temp > LRU2_BYTE_CNT_WIDTH)
                        overflow_time_byte ++;
                    if(PktCnt_temp > LRU2_PKT_CNT_WIDTH)
                        overflow_time_pkt ++;
                    if(!(symb_value_byte[Flow_ID_temp] + symb_value_pkt[Flow_ID_temp]))
                    {
                        sram_consumption ++;
                    }
                    //overflow
                    //upscale SRAM byte counter
                    if(scale_value_byte[Flow_ID_temp] == 0)
                    {
                        symb_value_byte[Flow_ID_temp] += (long)ByteCnt_temp;
                        if(symb_value_byte[Flow_ID_temp] > 65535)
                        {
                            scale_value_byte[Flow_ID_temp] = 1;
                            symb_value_byte[Flow_ID_temp]  = OEFUpdate(0, symb_value_byte[Flow_ID_temp], scale_byte_15);
                            upscale_cnt_byte++;
                        }
                    }
                    else if(scale_value_byte[Flow_ID_temp] == 1)//scale_byte_15
                    {
                        symb_value_byte[Flow_ID_temp] += OEFUpdate(symb_value_byte[Flow_ID_temp], (long)ByteCnt_temp, scale_byte_15);
                        if(symb_value_byte[Flow_ID_temp] > 209345)
                        {
                            scale_value_byte[Flow_ID_temp] = 2;
                            symb_value_byte[Flow_ID_temp] = OEF_renor(scale_byte_15, scale_byte_12, symb_value_byte[Flow_ID_temp]);
                            upscale_cnt_byte++;
                        }
                    }
                    else if(scale_value_byte[Flow_ID_temp] == 2)//scale_byte_12
                    {
                        symb_value_byte[Flow_ID_temp] += OEFUpdate(symb_value_byte[Flow_ID_temp], (long)ByteCnt_temp, scale_byte_12);
                        if(symb_value_byte[Flow_ID_temp] > 36322063329)
                        {
                            scale_value_byte[Flow_ID_temp] = 3;
                            symb_value_byte[Flow_ID_temp] = OEF_renor(scale_byte_12, scale_byte_11, symb_value_byte[Flow_ID_temp]);
                            upscale_cnt_byte++;
                        }
                    }
                    else if(scale_value_byte[Flow_ID_temp] == 3)//scale_byte_11
                    {
                        symb_value_byte[Flow_ID_temp] += OEFUpdate(symb_value_byte[Flow_ID_temp], (long)ByteCnt_temp, scale_byte_11);
                    }

                    if(scale_value_pkt[Flow_ID_temp] == 0)
                    {
                        symb_value_pkt[Flow_ID_temp] += (long)PktCnt_temp;
                        if(symb_value_pkt[Flow_ID_temp] > 4095)
                        {
                            scale_value_pkt[Flow_ID_temp] = 1;
                            symb_value_pkt[Flow_ID_temp]  = OEFUpdate(0, symb_value_pkt[Flow_ID_temp], scale_pkt_11);
                            upscale_cnt_pkt++;
                        }
                    }

                    //upscale packet counter
                    else if(scale_value_pkt[Flow_ID_temp] == 1)//scale_pkt_11
                    {
                        symb_value_pkt[Flow_ID_temp] += OEFUpdate(symb_value_pkt[Flow_ID_temp], (long)PktCnt_temp, scale_pkt_11);
                        if(symb_value_pkt[Flow_ID_temp] > 13073)
                        {
                            scale_value_pkt[Flow_ID_temp] = 2;
                            symb_value_pkt[Flow_ID_temp] = OEF_renor(scale_pkt_11, scale_pkt_10, symb_value_pkt[Flow_ID_temp]);
                            upscale_cnt_pkt++;
                        }
                    }
                    else if(scale_value_pkt[Flow_ID_temp] == 2)//scale_pkt_10
                    {
                        symb_value_pkt[Flow_ID_temp] += OEFUpdate(symb_value_pkt[Flow_ID_temp], (long)PktCnt_temp, scale_pkt_10);
                        if(symb_value_pkt[Flow_ID_temp] > 54747)
                        {
                            scale_value_pkt[Flow_ID_temp] = 3;
                            symb_value_pkt[Flow_ID_temp] = OEF_renor(scale_pkt_10, scale_pkt_8, symb_value_pkt[Flow_ID_temp]);
                            upscale_cnt_pkt++;
                        }
                    }
                    else if(scale_value_pkt[Flow_ID_temp] == 3)//scale_pkt_8
                    {
                        symb_value_pkt[Flow_ID_temp] += OEFUpdate(symb_value_pkt[Flow_ID_temp], (long)PktCnt_temp, scale_pkt_8);
                    }
                    ByteCnt_temp = 0;
                    PktCnt_temp = 0;
                    sram_trans += 2;
                    access_time += CALC_TIME + OFF_CHIP_TIME;
                }

                delete_node_pointed(list_head_2_p, found_item->hash_to_list);
                delete_user(found_item);


                cache_node = new dl_list_node;
                init_dl_list_node(cache_node, Flow_ID_temp, ByteCnt_temp, PktCnt_temp, list_head_2_p);
                insert_node_head(list_head_2_p, cache_node);
                add_user(Flow_ID_temp, cache_node);

                /*throughput*/
                access_time += ON_CHIP_TIME*2;
            }
            else
            {
                printf("errors occur!!!\n");
            }

            cache_hit ++;

        }
    }

    //write LRU_1 to off-chip DRAM
    cache_node = list_head_1.head;
    while(cache_node != NULL)
    {
        //20180205 debug
        dram_consumption ++;
        dram_trans += 1;

        ByteCnt_temp = cache_node->cached_value;
        PktCnt_temp = cache_node->cached_pkt;
        if(ByteCnt_temp > LRU1_byte_counter_max)
            LRU1_byte_counter_max = ByteCnt_temp;
        if(PktCnt_temp > LRU1_pkt_counter_max)
            LRU1_pkt_counter_max = PktCnt_temp;
        temp_key = cache_node->cached_key;
        pile_value_byte[temp_key] += ByteCnt_temp;
        pile_value_pkt[temp_key] += PktCnt_temp;
        cache_node = cache_node->next;
    }

    //write LRU_2 to off-chip SRAM
    cache_node = list_head_2.head;
    while(cache_node != NULL)
    {
        ByteCnt_temp = cache_node->cached_value;
        PktCnt_temp = cache_node->cached_pkt;
        if(symb_value_byte[cache_node->cached_key] == 0)
            sram_consumption ++;
        if(scale_value_byte[cache_node->cached_key] == 0)
        {
            symb_value_byte[cache_node->cached_key] += (long)ByteCnt_temp;
            if(symb_value_byte[cache_node->cached_key] > 65535)
            {
                scale_value_byte[cache_node->cached_key] = 1;
                symb_value_byte[cache_node->cached_key]  = OEFUpdate(0, symb_value_byte[cache_node->cached_key], scale_byte_15);
                    upscale_cnt_byte++;
                }
        }
        else if(scale_value_byte[cache_node->cached_key] == 1)//scale_byte_15
        {
            symb_value_byte[cache_node->cached_key] += OEFUpdate(symb_value_byte[cache_node->cached_key], (long)ByteCnt_temp, scale_byte_15);
            if(symb_value_byte[cache_node->cached_key] > 209345)
            {
                scale_value_byte[cache_node->cached_key] = 2;
                symb_value_byte[cache_node->cached_key] = OEF_renor(scale_byte_15, scale_byte_12, symb_value_byte[cache_node->cached_key]);
                upscale_cnt_byte++;
            }
        }
        else if(scale_value_byte[cache_node->cached_key] == 2)//scale_byte_12
        {
            symb_value_byte[cache_node->cached_key] += OEFUpdate(symb_value_byte[cache_node->cached_key], (long)ByteCnt_temp, scale_byte_12);
            if(symb_value_byte[cache_node->cached_key] > 36322063329)
            {
                scale_value_byte[cache_node->cached_key] = 3;
                symb_value_byte[cache_node->cached_key] = OEF_renor(scale_byte_12, scale_byte_11, symb_value_byte[cache_node->cached_key]);
                upscale_cnt_byte++;
            }
        }
        else if(scale_value_byte[cache_node->cached_key] == 3)//scale_byte_11
        {
            symb_value_byte[cache_node->cached_key] += OEFUpdate(symb_value_byte[cache_node->cached_key], (long)ByteCnt_temp, scale_byte_11);
        }
        if(scale_value_pkt[cache_node->cached_key] == 0)
        {
            symb_value_pkt[cache_node->cached_key] += (long)PktCnt_temp;
            if(symb_value_pkt[cache_node->cached_key] > 4095)
            {
                scale_value_pkt[cache_node->cached_key] = 1;
                symb_value_pkt[cache_node->cached_key]  = OEFUpdate(0, symb_value_pkt[cache_node->cached_key], scale_pkt_11);
                upscale_cnt_pkt++;
            }
        }
        else if(scale_value_pkt[cache_node->cached_key] == 1)//scale_pkt_11
        {
            symb_value_pkt[cache_node->cached_key] += OEFUpdate(symb_value_pkt[cache_node->cached_key], (long)PktCnt_temp, scale_pkt_11);
            if(symb_value_pkt[cache_node->cached_key] > 13073)
            {
                scale_value_pkt[cache_node->cached_key] = 2;
                symb_value_pkt[cache_node->cached_key] = OEF_renor(scale_pkt_11, scale_pkt_10, symb_value_pkt[cache_node->cached_key]);
                upscale_cnt_pkt++;
            }
        }
        else if(scale_value_pkt[cache_node->cached_key] == 2)//scale_pkt_10
        {
            symb_value_pkt[cache_node->cached_key] += OEFUpdate(symb_value_pkt[cache_node->cached_key], (long)PktCnt_temp, scale_pkt_10);
            if(symb_value_pkt[cache_node->cached_key] > 54747)
            {
                scale_value_pkt[cache_node->cached_key] = 3;
                symb_value_pkt[cache_node->cached_key] = OEF_renor(scale_pkt_10, scale_pkt_8, symb_value_pkt[cache_node->cached_key]);
                upscale_cnt_pkt++;
            }
        }
        else if(scale_value_pkt[cache_node->cached_key] == 3)//scale_pkt_8
        {
            symb_value_pkt[cache_node->cached_key] += OEFUpdate(symb_value_pkt[cache_node->cached_key], (long)PktCnt_temp, scale_pkt_8);
        }
        cache_node = cache_node->next;
        sram_trans += 2;
    }


<<<<<<< HEAD
    FILE * fp_1 = fopen("data\\output\\upscaled_cached_DISCO_byte.txt","w");
    FILE * fp_2 = fopen("data\\output\\uncached_DISCO_byte.txt","w");
=======
    temp_string = base_folder + "data\\output\\upscaled_cached_DISCO_byte.txt";
    FILE * fp_1 = fopen(temp_string.c_str(),"w");
    temp_string = base_folder + "data\\output\\uncached_DISCO_byte.txt";
    FILE * fp_2 = fopen(temp_string.c_str(),"w");
>>>>>>> lru2logic
    double esti_value_byte = 0;
    double esti_error_byte = 0;

    /*error_rate*/
    double rel_error_cached_byte = 0;
    double max_error_cached_byte = 0;
    double rel_error_un_byte     = 0;
    double max_error_un_byte     = 0;



    for(i = 0;i <= total_flow_cnt;i++)
    {
        //cached
        if(scale_value_byte[i] == 0)
        {
            esti_value_byte = symb_value_byte[i];
            esti_value_byte += pile_value_byte[i];
        }
        else if(scale_value_byte[i] == 1)
        {
            esti_value_byte = FC_OEF(symb_value_byte[i], scale_byte_15);
            esti_value_byte += pile_value_byte[i];
        }
        else if(scale_value_byte[i] == 2)
        {
            esti_value_byte = FC_OEF(symb_value_byte[i], scale_byte_12);
            esti_value_byte += pile_value_byte[i];
        }
        else if(scale_value_byte[i] == 3)
        {
            esti_value_byte = FC_OEF(symb_value_byte[i], scale_byte_11);
            esti_value_byte += pile_value_byte[i];
        }

        esti_error_byte = (esti_value_byte - real_value_byte[i])/real_value_byte[i];
        if(esti_error_byte < 0)
        {
            esti_error_byte *= -1;
        }
        /*error_rate*/
        rel_error_cached_byte += esti_error_byte;
        if(esti_error_byte >= max_error_cached_byte)
        max_error_cached_byte = esti_error_byte;

        fprintf(fp_1, "%ld,%ld,%ld,%lf,%lf\n", symb_value_byte[i], pile_value_byte[i], real_value_byte[i], esti_value_byte, esti_error_byte);

        //uncached
        esti_value_byte = FC_OEF(u_symb_value_byte[i], scale_byte);
        esti_error_byte = (esti_value_byte - real_value_byte[i])/real_value_byte[i];
        if(esti_error_byte < 0)
        {
            esti_error_byte *= -1;
        }
        /*error_rate*/
        rel_error_un_byte += esti_error_byte;
        if(esti_error_byte >= max_error_un_byte)
        max_error_un_byte = esti_error_byte;


        fprintf(fp_2, "%ld %lf %lf\n", real_value_byte[i], esti_value_byte, esti_error_byte);
    }

    /*error_rate*/
    rel_error_cached_byte = rel_error_cached_byte/(double)(total_flow_cnt+1);
    rel_error_un_byte     = rel_error_un_byte/(double)(total_flow_cnt+1);

    temp_string = base_folder + "data\\output\\upscaled_cached_DISCO_pkt.txt";
    FILE * fp_3 = fopen(temp_string.c_str(),"w");
    temp_string = base_folder + "data\\output\\uncached_DISCO_pkt.txt";
    FILE * fp_4 = fopen(temp_string.c_str(),"w");

<<<<<<< HEAD
    FILE * fp_3 = fopen("data\\output\\upscaled_cached_DISCO_pkt.txt","w");
    FILE * fp_4 = fopen("data\\output\\uncached_DISCO_pkt.txt","w");
=======
>>>>>>> lru2logic
    double esti_value_pkt = 0;
    double esti_error_pkt = 0;

    /*error_rate*/
    double rel_error_cached_pkt = 0;
    double max_error_cached_pkt = 0;
    double rel_error_un_pkt     = 0;
    double max_error_un_pkt     = 0;



    for(i = 0;i <= total_flow_cnt;i++)
    {
        //cached
        if(scale_value_pkt[i] == 0)
        {
            esti_value_pkt = symb_value_pkt[i];
            esti_value_pkt += pile_value_pkt[i];
        }
        else if(scale_value_pkt[i] == 1)
        {
            esti_value_pkt = FC_OEF(symb_value_pkt[i], scale_pkt_11);
            esti_value_pkt += pile_value_pkt[i];
        }
        else if(scale_value_pkt[i] == 2)
        {
            esti_value_pkt = FC_OEF(symb_value_pkt[i], scale_pkt_10);
            esti_value_pkt += pile_value_pkt[i];
        }
        else if(scale_value_pkt[i] == 3)
        {
            esti_value_pkt = FC_OEF(symb_value_pkt[i], scale_pkt_8);
            esti_value_pkt += pile_value_pkt[i];
        }

        esti_error_pkt = (esti_value_pkt - real_value_pkt[i])/real_value_pkt[i];
        if(esti_error_pkt < 0)
        {
            esti_error_pkt *= -1;
        }
        /*error_rate*/
        rel_error_cached_pkt += esti_error_pkt;
        if(esti_error_pkt >= max_error_cached_pkt)
        max_error_cached_pkt = esti_error_pkt;

        fprintf(fp_3, "%ld,%ld,%ld,%lf,%lf\n", symb_value_pkt[i], pile_value_pkt[i], real_value_pkt[i], esti_value_pkt, esti_error_pkt);

        //uncached
        esti_value_pkt = FC_OEF(u_symb_value_pkt[i], scale_pkt);
        esti_error_pkt = (esti_value_pkt - real_value_pkt[i])/real_value_pkt[i];
        if(esti_error_pkt < 0)
        {
            esti_error_pkt *= -1;
        }
        /*error_rate*/
        rel_error_un_pkt += esti_error_pkt;
        if(esti_error_pkt >= max_error_un_pkt)
        max_error_un_pkt = esti_error_pkt;


        fprintf(fp_4, "%ld %lf %lf\n", real_value_pkt[i], esti_value_pkt, esti_error_pkt);
    }

    /*error_rate*/
    rel_error_cached_pkt = rel_error_cached_pkt/(double)(total_flow_cnt+1);
    rel_error_un_pkt     = rel_error_un_pkt/(double)(total_flow_cnt+1);

    double aver_pkt_size = total_byte_cnt/(double)total_pkt_cnt;
    /*throughput*/
    double mpps = 0;
    mpps = (double)total_pkt_cnt*pow(10.0, 9.0)/1024.0/1024.0/access_time;
    double gbps = 0;
    gbps = total_byte_cnt*pow(10.0, 9.0)*8.0/1024.0/1024.0/1024.0/access_time;
    double mpps_un = 0;
    mpps_un = pow(10.0, 9.0)/1024.0/1024.0/(double)(OFF_CHIP_TIME*2 + CALC_TIME);
    double gbps_un = 0;
    gbps_un = pow(10.0, 9.0)/1024.0/1024.0/1024.0/(double)(OFF_CHIP_TIME*2 + CALC_TIME)*(double)aver_pkt_size*8.0;

    printf("LRU_1 module's byte counter's max value = %10d.\n", LRU1_byte_counter_max);
    printf("LRU_1 module's packet counter's max value = %10d.\n", LRU1_pkt_counter_max);
    printf("total byte count is = %10f.\n", total_byte_cnt);
    printf("average packet size is = %10f.\n", aver_pkt_size);
    printf("access time is = %10f.\n", access_time);
    printf("dram consumption is = %d.\n", dram_consumption);
    printf("dram transport times is = %d.\n", dram_trans);
    printf("sram consumption is = %d.\n", sram_consumption);
    printf("sram transport times is = %d.\n", sram_trans);

    printf("number of flows = %d.\n", total_flow_cnt+1);
    printf("number of packets = %d.\n", total_pkt_cnt);
    printf("pkt counter overflow times = %10d.\n", overflow_time_pkt);
    printf("byte counter overflow times = %10d.\n", overflow_time_byte);
    printf("cache hit times = %d, cache miss time = %d.\n", cache_hit, cache_miss);
    cache_hit_rate = (float)cache_hit/(cache_hit + cache_miss);
    printf("cache hit rate = %f.\n", cache_hit_rate);
    printf("upscaling count of byte = %d.\n", upscale_cnt_byte);
    printf("upscaling count of packet = %d.\n", upscale_cnt_pkt);

    /*error_rate*/
    printf("rel_error_uncached_byte = %.10lf.\n", rel_error_un_byte);
    printf("max_error_uncached_byte = %.10lf.\n", max_error_un_byte);
    printf("rel_error_cached_byte = %.10lf.\n", rel_error_cached_byte);
    printf("max_error_cached_byte = %.10lf.\n", max_error_cached_byte);
    printf("rel_error_uncached_pkt = %.10lf.\n", rel_error_un_pkt);
    printf("max_error_uncached_pkt = %.10lf.\n", max_error_un_pkt);
    printf("rel_error_cached_pkt = %.10lf.\n", rel_error_cached_pkt);
    printf("max_error_cached_pkt = %.10lf.\n", max_error_cached_pkt);

    /*throughput*/
    printf("cached throughput Mpps = %lf.\n", mpps);
    printf("cached throughput Gbps = %lf.\n", gbps);
    printf("uncached throughput Mpps = %lf.\n", mpps_un);
    printf("uncached throughput Gbps = %lf.\n", gbps_un);

    fclose(fp);
    fclose(fp_1);
    fclose(fp_2);
    fclose(fp_3);
    fclose(fp_4);
    return 0;
}
