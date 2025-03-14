#include <stdbool.h>
#include <stdint.h>

// ��Ϊ������1-Wire�����ļ�Ӧ����Ҫ�ṩdelay_us�����Ͳ�ͬ������1-Wire��/дһλ���ݺ����Ļص��ӿ�

/*                                    �ӿ�����                                    */

/*
 * 1-Wire ���߳�ʼ������
 * @brief  ʹ�ܲ���ʼ�� 1-Wire ���� GPIO ��
 * @param  void
 * @retval void
 */
void Onewire_Init(void);

/*
 * 1-Wire ���߸�λ����
 * @brief  ���͸�λ���岢��ȡӦ���ź�
 * @param  rst_time: ��λ����ĳ���ʱ��-us
 * @param  rsp_time: �ӻ���Ӧ�źų���ʱ��-us
 * @param  wait_time: Ӧ���źŽ������ӻ��ͷ����ߵĵȴ�ʱ��-us
 * @retval 1: Ӧ��ɹ�
 *         0: Ӧ��ʧ��
 */
bool Onewire_Reset(uint16_t rst_time, uint16_t rsp_time, uint16_t wait_time);

/*
 * 1-Wire ������д��һ���ֽ�
 * @brief  ���õ�λ���У�LSB����ʽд��һ���ֽ�
 * @param  data: д����ֽ�����
 * @retval void
 */
void Onewire_WriteByte(uint8_t data);

/*
 * 1-Wire �����϶�ȡһ���ֽ�
 * @brief  ���õ�λ���У�LSB����ʽ��ȡһ���ֽ�
 * @param  void
 * @retval ��ȡ���ֽ�����
 */
uint8_t Onewire_ReadByte(void);
