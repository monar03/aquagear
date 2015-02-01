<?php
namespace aquagear\model;

/**
 * XMLのデータ取得のためのコネクタ.
 * 
 * @package aquagear 
 * @subpackage model
 * 
 * @author motonari
 * @version $Id: XmlModelConnector.inc 535 2011-07-26 14:02:27Z motonari $
 */
class XmlModelConnector extends Model implements IModelConnector {
	/**
	 * モデルの設定ファイル.
	 * 
	 * @var array
	 */
	private $_model_system_conf = null;
		
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#initConnection()
	 */
	public function initConnection(array $conf_arr) {
		$this->_model_system_conf = $conf_arr;
	}
	
	/**
	 * モデルインスタンスの作成.
	 * 
	 * @param string $section 設定ファイルのセクション名
	 * @return object
	 */
	public static function getModelInstance($section) {
		throw new \aquagear\exception\ModelException('can not execute '. __CLASS__ .'getModelInstance($section)', '500');
	}
	
	/**
	 * XML文字列からインスタンスをロードする.
	 * 
	 * @param string $data xmlストリング
	 * 
	 * @return obj SimpleXMLオブジェクト,失敗時false
	 */
	public static function loadXmlFromString($data) {
		if(! empty($data)) {
			return simplexml_load_string($data);
		}
		else {
			return false;
		}
	}
	
	/**
	 * XMLファイルからインスタンスをロードする.
	 * 
	 * @param string $path xmlファイルパス
	 * 
	 * @return obj SimpleXMLオブジェクト,失敗時false
	 */
	public static function loadXmlFromFile($path) {
		if(is_file($path)) {
			return simplexml_load_file($path);
		}
		else {
			return false;
		}
	}
	
	/**
	 * URLかどうかのチェック
	 * 
	 * @param string $str チェックする文字列
	 * 
	 * @return bool URLの場合true,それ以外false
	 */
	private function isUrl($str) {
		if(preg_match('/^http(s)?\:\/\//', $str) === 0) {
			return false;
		}
		else {
			return true;
		}
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#setValue($conf_section, $param)
	 */
	public function setValue($conf_section, array $param) {
		$conf_arr = $this->getApplictionConf($conf_section);
		if($this->isUrl($conf_arr['source'])) {
			$url = $conf_arr['source'];
			if($conf_arr['method'] === 'get') {
				$get_param = http_build_query($param);
				if(!empty($get_param)) {
					$url .= '?' . $get_param;
				}
			}
			
			$curl = new \aquagear\http\Curl($url);
			
			if($conf_arr['method'] === 'post') {
				$curl->setOption(CURLOPT_POST, 1);
				$curl->setOption(CURLOPT_POSTFIELDS, http_build_query($param));
			}
			else if($conf_arr['method'] === 'file'){
				$curl->setOption(CURLOPT_POST, 1);
				$curl->setOption(CURLOPT_POSTFIELDS, $param);
			}

			$model_instance = self::loadXmlFromString($curl->execute());
		}
		else {
			if(is_file($conf_arr['source'])) {
				$model_instance = self::loadXmlFromFile($conf_arr['source']);
			}
			else {		
				return false;
			}
		}
		
		return $model_instance;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#getValue($conf_section, $param)
	 */
	public function getValue($conf_section, array $param=array()) {
		if(isset($this->_cache_data[md5($conf_section . var_export($param, true))])) {
			return $this->_cache_data[md5($conf_section . var_export($param, true))];
		}
		
		$data = $this->setValue($conf_section, $param);
		$this->_cache_data[md5($conf_section . var_export($param, true))] = $data;
		
		return $this->_cache_data[md5($conf_section . var_export($param, true))];
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/model/\aquagear\model\IModelConnector#deleteValue($conf_section, $param)
	 */
	public function deleteValue($conf_section, array $param=array()) {
		return $this->setValue($conf_section, $param);
	}
}
?>