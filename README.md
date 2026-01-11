# Purpose of FlashEEPROM

The purpose of this library is to enable STM32F1xx devices to store more than 2 KB of data in EEPROM (Flash).

The original library was limited to writing data to a single page and committed changes to flash memory for every byte written. This approach was inefficient and caused unnecessary wear on the flash. In contrast, this library is designed to be more efficient: it only commits data to EEPROM when explicitly requested by the code or when data must move to the next page.

As a result, flash wear is significantly reduced and overall performance is improved. For example, writing an object that spans an entire page could take more than five minutes with the original library, whereas this library completes the same operation in just a few seconds.


## Original library
Original library can be found here https://github.com/stm32duino/Arduino_Core_STM32/tree/main/libraries/EEPROM


# **Library functions**

#### **`FlashEEPROM.read( address )`**

This function allows you to read a single byte of data from the Flasheeprom.
Its only parameter is an `int` which should be set to the address you wish to read.

The function returns an `unsigned char` containing the value read.

#### **`FlashEEPROM.write( address, value )`**

The `write()` method allows you to write a single byte of data to the FlashEEPROM.
Two parameters are needed. The first is an `int` containing the address that is to be written, and the second is a the data to be written (`unsigned char`).

This function does not return any value.

#### **`FlashEEPROM.update( address, value )`**

This function is similar to `FlashEEPROM.write()` however this method will only write data if the cell contents pointed to by `address` is different to `value`. This method can help prevent unnecessary wear on the FlashEEPROM cells.

This function does not return any value.

#### **`FlashEEPROM.get( address, object )`**

This function will retrieve any object from the FlashEEPROM.
Two parameters are needed to call this function. The first is an `int` containing the address that is to be written, and the second is the object you would like to read.

This function returns a reference to the `object` passed in. It does not need to be used and is only returned for conveience.

#### **`FlashEEPROM.put( address, object )`**

This function will write any object to the FlashEEPROM.
Two parameters are needed to call this function. The first is an `int` containing the address that is to be written, and the second is the object you would like to write.

This function uses the _update_ method to write its data, and therefore only rewrites changed cells.

This function returns a reference to the `object` passed in. It does not need to be used and is only returned for conveience.

#### **Subscript operator: `FlashEEPROM[address]`**

This operator allows using the identifier `FlashEEPROM` like an array.  
FlashEEPROM cells can be read _and_ **_written_** directly using this method.

This operator returns a reference to the FlashEEPROM cell.

```c++
unsigned char val;

//Read first FlashEEPROM cell.
val = FlashEEPROM[ 0 ];

//Write first FlashEEPROM cell.
FlashEEPROM[ 0 ] = val;

//Compare contents
if( val == FlashEEPROM[ 0 ] ){
  //Do something...
}
```

#### **`FlashEEPROM.length()`**

This function returns an `unsigned int` containing the number of cells in the FlashEEPROM.

---

### **Advanced features**

This library uses a component based approach to provide its functionality. This means you can also use these components to design a customized approach. Two background classes are available for use: `EERef` & `EEPtr`.

#### **`EERef` class**

This object references an FlashEEPROM cell.
Its purpose is to mimic a typical byte of RAM, however its storage is the FlashEEPROM.
This class has an overhead of two bytes, similar to storing a pointer to an FlashEEPROM cell.

```C++
EERef ref = FlashEEPROM[ 10 ]; //Create a reference to 11th cell.

ref = 4; //write to FlashEEPROM cell.

unsigned char val = ref; //Read referenced cell.
```

#### **`EEPtr` class**

This object is a bidirectional pointer to FlashEEPROM cells represented by `EERef` objects.
Just like a normal pointer type, this type can be dereferenced and repositioned using 
increment/decrement operators.

```C++
EEPtr ptr = 10; //Create a pointer to 11th cell.

*ptr = 4; //dereference and write to FlashEEPROM cell.

unsigned char val = *ptr; //dereference and read.

ptr++; //Move to next FlashEEPROM cell.
```

#### **`FlashEEPROM.begin()`**

This function returns an `EEPtr` pointing to the first cell in the FlashEEPROM.  
This is useful for STL objects, custom iteration and C++11 style ranged for loops.

#### **`FlashEEPROM.end()`**

This function returns an `EEPtr` pointing at the location after the last FlashEEPROM cell.  
Used with `begin()` to provide custom iteration.

**Note:** The `EEPtr` returned is invalid as it is out of range. In fact the hardware causes wrapping of the address (overflow) and `FlashEEPROM.end()` actually references the first FlashEEPROM cell.
