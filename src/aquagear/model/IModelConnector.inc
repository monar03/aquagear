<?php 
namespace aquagear\model;

/**
 * モデルの基底クラス.
 * 
 * @package aquagear 
 * @subpackage model
 * 
 * @author motonari
 * @version $Id: IModelConnector.inc 534 2011-07-26 13:48:33Z motonari $
 */
interface IModelConnector  {
	/**
	 * コネクションの初期化.
	 * 
	 * @param array $conf_arr 設定ファイルの配列
	 * @return void
	 */
	 public function initConnection(array $conf_arr);
	
	 /**
	  * 値を設定する.
	  * 
	  * @param string $conf_section セクション
	  * @param array $param パラメータ
	  * 
	  * @return bool 成功時true,失敗時false
	  */
	 public function setValue($conf_section, array $param);

	 /**
	  * 値を取得する.
	  * 
	  * @param string $conf_section セクション
	  * @param array $param パラメータ
	  * 
	  * @return bool 成功時true,失敗時false
	  */
	 public function getValue($conf_section, array $param=array());
	 
	 /**
	  * 値の削除をする.
	  * 
	  * @param string $conf_section セクション
	  * @param array $param パラメータ
	  * 
	  * @return bool 成功時true,失敗時false
	  */
	 public function deleteValue($conf_section, array $param=array());
}
?>