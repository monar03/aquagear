<?php 
namespace aquagear\validate;

/**
 * 文字列関係のvalidate.
 * 
 * @package aquagear 
 * @subpackage validate
 * 
 * @author motonari
 * @version $Id: StringValidator.inc 535 2011-07-26 14:02:27Z motonari $
 */
class StringValidator extends CoreValidator {
	/**
	 * 文字列の長さの調査
	 * @param string $value 確認する値
	 * @param int $length 長さ
	 * @return bool
	 */
	public static function stringLength($value, $length) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if(strlen(htmlspecialchars_decode($value)) <= $length) {
			return true;
		}
		else {
			return false;
		}
	}
	
	/**
	 * 正規表現による、文字列のマッチ.
	 * 
	 * @param string $value 確認する値
	 * @param string $pattarn 確認するパターン
	 * @return bool
	 */
	public static function matchRegString($value, $pattarn) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if(preg_match($pattarn, $value)) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * 正規表現による、文字列のマッチ.
	 * 
	 * @param string $value 確認する値
	 * @param string $pattarn 確認するパターン
	 * @return bool
	 */
	public static function noMatchRegString($value, $pattarn) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if(preg_match($pattarn, $value)) {
			return false;
		}
		else {
			return true;
		}
	}
	
	/**
	 * 文字列のマッチ.
	 * 
	 * @param string $value 確認する値
	 * @param string $string マッチする文字列
	 * @return bool
	 */
	public static function matchString($value, $string) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if(strstr($value, $string) !== false) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * 文字列のマッチ.
	 * 
	 * @param string $value 確認する値
	 * @param string $string マッチする文字列
	 * @return bool
	 */
	public static function noMatchString($value, $string) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if(strstr($value, $string) !== false) {
			return false;
		}
		else {
			return true;
		}
	}	
	
	/**
	 * 区別なしの正規表現による、文字列チェック.
	 * 
	 * @param string $value 確認する値
	 * @param string $pattarn 確認するパターン
	 * @return bool
	 */
	public static function imatchRegString($value, $pattarn) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if(preg_match($pattarn, strtolower($value))) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * 区別なしの正規表現による、文字列チェック.
	 * 
	 * @param string $value 確認する値
	 * @param string $pattarn 確認するパターン
	 * @return bool
	 */
	static public function inoMatchRegString($value, $pattarn) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if(preg_match($pattarn, strtolower($value))) {
			return false;
		}
		else {
			return true;
		}
	}
	
	/**
	 * 区別なしの、文字列チェック.
	 * 
	 * @param string $value 確認する値
	 * @param string $string マッチする文字列
	 * @return bool
	 */
	public static function imatchString($value, $string) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if(stristr($value, $string) !== false) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * 区別なしの、文字列チェック.
	 * 
	 * @param string $value 確認する値
	 * @param string $string マッチする文字列
	 * @return bool
	 */
	public static function inoMatchString($value, $string) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if(stristr($value, $string) !== false) {
			return false;
		}
		else {
			return true;
		}
	}
}
?>