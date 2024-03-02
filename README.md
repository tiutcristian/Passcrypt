# Passcrypt

## Description

Passcrypt serves as a reliable password management solution meticulously created to safeguard your digital existence. The inspiration behind the development of this application comes from my wish to keep users' valuable information safe. Passcrypt stores passwords in an encrypted format on your local machine, meaning that you have access to your passwords at any time.

The project provided an awesome learning experience for me, encompassing programming proficiency, framework utilization and external libraries incorporation.

1. C++ Proficiency: The project involved a deep dive into C++ programming, enhancing proficiency in the language, this including a solid understanding of object-oriented programming concepts as well as external libraries usage.

2. Qt Framework Mastery: Working with the Qt Framework provided insights into transforming a basic console-based application into an intuitive GUI-driven program. Learning to leverage Qt's libraries and tools facilitated the creation of a user-friendly interface and streamlined the development process.

3. Security Implementation: The process of securing sensitive information was a significant learning aspect.

## Installation

- Download the release from the [Releases section](https://github.com/tiutcristian/Passcrypt/releases/) and run Passcrypt.exe.

## Usage 

### 1. Menu

![Required image is currently not available](assets/images/homepage.png)

When starting the app you will be greeted with the home page, accessible only when opening the app. Then you can use the buttons in the side bar to navigate through the app and use its features:
- Database
- Fast generate
- Settings
- Help

### 2. Database

![Required image is currently not available](assets/images/database.png)

In the database page you can visualize your passwords in a table and copy, edit or delete them.
You can add a new password using ‘Auto’ or ‘Manual’ buttons. 
        
1. ‘Auto’ will automatically generate a new complex password that you can save to the database and immediately use it. In advanced section you can customize the way in which the password is generated (how many characters, which characters)
2. ‘Manual’ will provide you with a form similar to the one in ‘Auto’, except this time you will have to manually introduce a password.

![Required image is currently not available](assets/images/add.png)

The password database is encrypted using stream ciphers from libsodium and is stored locally in a file.

### 3. Fast generate
In fast generate page you can generate passwords and copy them for immediate use.
These passwords will not be saved to the database.

![Required image is currently not available](assets/images/fastgen.png)

### 4. Settings
    
In settings you can change your master password. You must enter your current master password and then enter your new password twice. After clicking 'Change password' the database will be encrypted with respect to your new master password and accessing the app will require you to remember your new master password.
