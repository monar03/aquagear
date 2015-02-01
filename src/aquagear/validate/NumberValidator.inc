<?php
namespace aquagear\validate;

/**
 * 数値関係のvalidate.
 * 
 * @package aquagear 
 * @subpackage validate
 * 
 * @author motonari
 * @version $Id: NumberValidator.inc 535 2011-07-26 14:02:27Z motonari $
 */ 
class NumberValidator extends CoreValidator {
	/**
	 * 数値のマッチ.
	 * 
	 * @param string $value 確認する値
	 * @param int $number マッチさせる数値
	 * @return bool
	 */
	public static function matchNumber($value, $number) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if($value === (int) $number) {
			return true;
		}
		else {
			return false;
		}
	}
	
	/**
	 * 数値のマッチ.
	 * 
	 * @param string $value 確認する値
	 * @param int $number マッチさせる数値
	 * @return bool
	 */
	public static function noMatchNumber($value, $number) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if($value === (int) $number) {
			return false;
		}
		else {
			return true;
		}
	}
	
	/**
	 * 数字のみかどうか.
	 * 
	 * @param string $value 確認する値
	 * @return bool
	 */
	public static function isNumeric($value) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if($value === '') {
			return true;
		}
		
		if(is_numeric($value)) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * 数字のみかどうか.
	 * 
	 * @param string $value 確認する値
	 * @return bool
	 */
	public static function isNoNumeric($value) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if($value === '') {
			return true;
		}
		
		if(is_numeric($value)) {
			return false;
		}
		else {
			return true;
		}
	}
	
	/**
	 * 数値の範囲.
	 * 
	 * @param string $value 確認する値
	 * @param array $numbers_arr 数値の範囲の配列
	 * @return bool
	 */
	public static function rangeNumber($value, $numbers_arr) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if($numbers_arr[0] <= $value && $value <= $numbers_arr[1]) { 
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * 数値の範囲.
	 * 
	 * @param string $value 確認する値
	 * @param array $numbers_arr 数値の範囲の配列
	 * @return bool
	 */
	public static function noRangeNumber($value, $numbers_arr) {
		\aquagear\log\Logger::getLogInstance()->debug(__CLASS__ . " : " . __FUNCTION__ . " start.");
		if($numbers_arr[0] <= $value && $value <= $numbers_arr[1]) { 
			return false;
		}
		else {
			return true;
		}
	}
}
?>