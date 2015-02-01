<?php
namespace aquagear\cache;

/** 
 * キャッシュクラス用テンプレート
 * 
 * @package aquagear
 * @subpackage cache
 * 
 * @author motonari
 * @version $Id: ICache.inc 534 2011-07-26 13:48:33Z motonari $
 * 
 */
interface ICache {

	/**
	 * キャッシュの初期化メソッド.
	 * 
	 * @param mixed $property プロパティ
	 * @param array $conf_arr 設定ファイル配列
	 * @return void
	 */
	public function initCache(array $conf_arr);
	
	/**
	 * キャッシュに値のセット.
	 * 
	 * @param string $key キー
	 * @param mixed  $value 値
	 * @param int    $ttl キャッシュの存在時間
	 * @return bool
	 */
	public function setValue($key, $value, $ttl=0);
	
	/**
	 * 値の取得.
	 * 
	 * @param string $key キー
	 * @return mixed 値
	 */
	public function getValue($key);
	
	/**
	 * 値の削除.
	 * 
	 * @param string $key キー
	 * @return bool 成功時true,失敗時false
	 */
	public function deleteValue($key);
	
	/**
	 * キャッシュの存在確認
	 * 
	 * @param $key
	 * @param $ttl
	 * @return unknown_type
	 */
	public function cacheExists($key, $ttl=0);
}
?>