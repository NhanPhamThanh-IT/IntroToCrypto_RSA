import sympy
import random
import inspect

print(inspect.getsource(sympy.isprime))


def checkprime(hex_string):
    # hex_string = "E34FCE0CDFEA1A3B7FC5D31978CDFE3C3DFD275C3A5ACDC2D8B754A9A7B5E43DF1F3B5C8C43FEF214D7B12C1C7ED75398C3C7F73F637CA7F14DBC4E191B2F99D7E81B6F768D19B39DA3F165A350C5F5F87FEFB4FF708B9A7AE79D8F376A4A3C01B49B8D66F709CAE"
    number = int(hex_string, 16)
    is_prime = sympy.isprime(number)
    return is_prime

def generate_large_prime_hex(bits=1024):
    # Tạo một số ngẫu nhiên có độ dài 1024 bit
    while True:
        random_number = random.getrandbits(bits)
        # Đảm bảo số là số lẻ (bắt buộc để có thể là số nguyên tố)
        random_number |= 1
        # Kiểm tra số đó có phải số nguyên tố không
        if sympy.isprime(random_number):
            # Trả về dạng chuỗi hex (256 ký tự với 1024 bit)
            return hex(random_number)[2:].upper()

prime_hex = generate_large_prime_hex(512)

print(prime_hex[::-1])

print(checkprime(prime_hex))