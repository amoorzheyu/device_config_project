#ifndef SLAVE_H
#define SLAVE_H

// 返回数组：[拼接值, 地址, 地址+1, ...]，需手动释放
int* get_slave_addresses(int device_id, int start_address, int length);

#endif
