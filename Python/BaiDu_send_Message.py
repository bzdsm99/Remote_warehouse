from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium import webdriver
from io import BytesIO
from PIL import Image
import ddddocr
import requests
import time

def BaiDu_sendMessage(phone_number):
    url = "https://www.baidu.com/"
    prefs = {"profile.default_content_setting_values": {"ssl": 2}}
    chrome_options = Options()
    # 禁用SSL验证
    prefs = {"profile.default_content_setting_values": {"ssl": 2}}
    chrome_options.add_experimental_option("prefs", prefs)
    #设置模拟窗口格式
    chrome_options.add_argument('--headless')
    chrome_options.add_argument('--disable-gpu')  # 在某些系统上禁用 GPU 可以避免问题
    chrome_options.add_argument('--no-sandbox') 
    driver = webdriver.Chrome(options=chrome_options)
    driver = webdriver.Chrome()
    # driver.set_window_size(1200, 800)
    # driver.implicitly_wait(10)
    driver.get(url)
    driver.find_element(By.LINK_TEXT, '登录').click()
    time.sleep(2)
    driver.find_element(By.CSS_SELECTOR, '#TANGRAM__PSP_11__changeSmsCodeItem').click()
    driver.find_element(By.CSS_SELECTOR, '.pass-text-input.pass-text-input-smsPhone').send_keys(phone_number)
    driver.find_element(By.CSS_SELECTOR, '.pass-item-timer').click()

    def login_get_code(verifyCode):
            #获取验证码截图处理
            time.sleep(2)
            x, y = verifyCode.location.values() # 坐标
            h, w = verifyCode.size.values() # 宽高
            image_data = driver.get_screenshot_as_png() # 把截图以二进制形式的数据返回
            screenshot = Image.open(BytesIO(image_data)) # 以新图片打开返回的数据
            result = screenshot.crop((x, y, x + w, y + h)) # 对截图进行裁剪
            result.save('Bai_Du_verifyCode.png')   # result.show() # 显示图片
            with open('Bai_Du_verifyCode.png', 'rb') as f:
                img_bytes = f.read()
            ocr = ddddocr.DdddOcr(show_ad=False)
            code = ocr.classification(img_bytes)
            print(f'验证码识别成功: {code}')
            return code
    def input_VerifyCode_again():
        try:
            verifyCode = driver.find_element(By.CSS_SELECTOR, '#TANGRAM__PSP_11__confirmVerifyCodeImg')
        except Exception as e:
            print("等待时间中 24小时")
            return "错误"
        driver.find_element(By.CSS_SELECTOR, '#TANGRAM__PSP_11__confirmVerifyCode').send_keys(login_get_code(verifyCode))
        driver.find_element(By.CSS_SELECTOR, '#TANGRAM__PSP_56__confirm_continue').click()
        driver.find_element(By.CSS_SELECTOR, '#TANGRAM__PSP_11__confirmVerifyCodeChange').click()
        error_text = driver.find_element(By.CSS_SELECTOR, '#TANGRAM__PSP_11__confirmVerifyCodeError').text
        print("error_text :",error_text,'***')
        return error_text

    error_text = input_VerifyCode_again()
    while error_text =="验证码错误":
        error_text = input_VerifyCode_again()

import re
def is_phone_number(phone_num):
    # 中国大陆手机号码一般以1开头，第二位通常是3到9之间的数字，后面跟着9位数字
    phone_pattern = re.compile(r'^1[3-9]\d{9}$')
    return bool(phone_pattern.match(phone_num))

# 示例
input_str = input("请输入手机号码: ")
if is_phone_number(input_str):
    print("手机号码有效")
    try:
        BaiDu_sendMessage(input_str)
    except Exception as e:
        print("短信发送失败")

else:
    print("输入的不是有效的手机号码")