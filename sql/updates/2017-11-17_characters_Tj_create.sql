/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50540
Source Host           : localhost:3306
Source Database       : jd2_auth

Target Server Type    : MYSQL
Target Server Version : 50540
File Encoding         : 65001

Date: 2017-11-17 15:35:48
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `tj`
-- ----------------------------
DROP TABLE IF EXISTS `tj`;
CREATE TABLE `tj` (
  `guid` int(12) NOT NULL,
  `acc_id` int(11) NOT NULL DEFAULT '0',
  `username` varchar(12) NOT NULL DEFAULT '',
  `Type` int(1) NOT NULL DEFAULT '1',
  `val` int(10) NOT NULL DEFAULT '0',
  `tiem` bigint(40) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tj
-- ----------------------------
