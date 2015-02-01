<?php 
namespace aquagear\validate;

/**
 * 存在の有無のvalidateクラス.
 * 
 * @package aquagear 
 * @subpackage validate
 * 
 * @author motonari
 * @version $Id: ExistsValidator.inc 534 2011-07-26 13:48:33Z motonari $
 */
class ExistsValidator extends CoreValidator {
	/**
	 * 存在の有無の確認.
	 * 
	 * @param string $value 確認する値
	 * 
	 * @return bool
	 */
	public static function isSetValue($value) {
		if($value === true) {
			return true;
		}
		else {
			return false;
		}
	}
	
	/**
	 * 存在の有無の確認.
	 * 
	 * @param string $value 確認する値
	 * 
	 * @return bool
	 */
	public static function isNotSetValue($value) {
		if($value === true) {
			return false;
		}
		else {
			return true;
		}
	}
	
	/**
	 * 空かどうか.
	 * 
	 * @param string $value 確認する値
	 * 
	 * @return bool
	 */
	public static function isEmpty($value) {
		$value = ltrim($value);
		if($value === '' ||
			$value === null ||
			$value === false) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * 空かどうか.
	 * 
	 * @param string $value 確認する値
	 * 
	 * @return bool
	 */
	public static function isNotEmpty($value) {
		$value = ltrim($value);
		if($value === '' ||
			$value === null ||
			$value === false) {
			return false;
		}
		else {
			return true;
		}
	}
}
?>