<?php 
namespace aquagear\holder;

/**
 * ホルダクラスの基底クラス.
 * 
 * @package aquagear
 * @subpackage holder
 * 
 * @author motonari
 * @version $Id: CoreHolder.inc 534 2011-07-26 13:48:33Z motonari $
 */
abstract class CoreHolder extends \aquagear\Object {
	/**
	 * 素のデータを取得するフィルター
	 * 
	 * @var int
	 */
	const NO_FILTER = 1;

	/**
	 * HTMLエンティティ化したデータを取得するフィルター
	 * 
	 * @var int
	 */
	const HTML_FILTER = 2;

	/**
	 * URLエンコードしたデータを取得するフィルター
	 * 
	 * @var int
	 */
	const URL_FILTER = 4;
	
	/**
	 * strip_tagを通した後のデータを取得するフィルター
	 * 
	 * @var int
	 */
	const STRIP_TAG_FILTER = 8;
	
	/**
	 * 値をフィルタリングをする。
	 * 
	 * @param mixed $value 対象の値
	 * @param int $filter フィルター
	 * 
	 * @return mixed フィルタリングの結果
	 */
	protected function filterValue($value, $filter=self::HTML_FILTER) {
		if(is_array($value)) {
			$result = array();
			foreach($value as $key => $val) {
				$result[$key] = $this->_convertString($val, $filter);
			}
			
			return $result;
		}
		else {
			return $this->_convertString($value, $filter);
		}
	}
	
	/**
	 * 値を変換する。
	 * 
	 * @param mixed $value 対象の値
	 * @param int $filter フィルター
	 * 
	 * @return mixed フィルタリングの結果
	 */
	private function _convertString($value, $filter=self::HTML_FILTER) {
		switch($filter) {
			case self::NO_FILTER:
				return $value;
			case self::HTML_FILTER:
				return htmlspecialchars($value);
			case self::URL_FILTER:
				return rawurlencode($value);
			case self::STRIP_TAG_FILTER:
				return strip_tags($value);
			default:
				return htmlspecialchars($value);
		}
	}
}
?>