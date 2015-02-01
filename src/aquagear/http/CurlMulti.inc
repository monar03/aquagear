<?php 
namespace aquagear\http;

/**
 * CurlMultiのクラス.
 * 
 * @package aquagear
 * @subpackage http
 * 
 * @author motonari
 * @version $Id: CurlMulti.inc 535 2011-07-26 14:02:27Z motonari $
 *
 */
class CurlMulti extends CoreHttpRequest {
	/**
	 * Curlオブジェクトの配列.
	 * 
	 * @var array
	 */
	private $_curl_obj_arr = array();
	
	/**
	 * curl_multi ハンドラー
	 * @var resource
	 */
	private $_multi_handler = null;
	
	/**
	 * コンストラクタ.
	 * 
	 * @return void
	 */
	public function __construct() {
		$this->open();
	}
	
	/**
	 * ハンドラーの設定.
	 * 
	 * @param string $key キー
	 * @param object $curl_obj Curl用オブジェクト
	 * @return bool
	 */
	public function addHandler($key ,\aquagear\http\Curl $curl_obj) {
		if(!empty($curl_obj)) {
			$this->_curl_obj_arr[$key] = $curl_obj;
			return true;
		}
		
		return false;
	}
	
	public function open() {
		if(empty($this->_multi_handler)) {
			$this->_multi_handler = curl_multi_init();
		}
	}
	
	/**
	 * 実行CurlMultiを実行する.
	 * 
	 * 
	 * @return bool
	 */
	public function execute() {
		\aquagear\log\Logger::getLogInstance()->info('curl multi execute.');
		
		foreach($this->_curl_obj_arr as $curl_obj) {
			$handler = $curl_obj->getHandler();
			if($handler) {
				curl_multi_add_handle($this->_multi_handler, $handler);
			}
		}
		
		$active = null;
		do {
			$mrc = curl_multi_exec($this->_multi_handler, $active);
		}while($mrc == CURLM_CALL_MULTI_PERFORM);
		
		while($active && $mrc == CURLM_OK) {
			if(curl_multi_select($this->_multi_handler) != -1) {
				do {
					$mrc = curl_multi_exec($this->_multi_handler, $active);
				}while($mrc == CURLM_CALL_MULTI_PERFORM);
			}
		}
		
		return $this->_checkError();
	}

	/**
	 * エラーのチェック.
	 * 
	 * @todo ここのロジックが怪しい.
	 * @return bool
	 */
	private function _checkError() {
		foreach ($this->_curl_obj_arr as $key => $curl_obj) {
			if(($err = $curl_obj->getErrno()) !== 0) {
				\aquagear\log\Logger::getLogInstance()->warning('curl error [key=' . $key . '][error_no = ' . $err . ']');
				return false;
			}
		}
		
		return true;
	}
		
	/**
	 * 情報の取得.
	 * 
	 * @param string $key キー
	 * @return string
	 */
	public function getContent($key) {
		return curl_multi_getcontent($this->_curl_obj_arr[$key]->getHandler());
	}
	
	/**
	 * ハンドラーを閉じる.
	 * 
	 * @return void
	 */
	public function close() {
		if(empty($this->_multi_handler)) {
			return ;
		}
		
		foreach($this->_curl_obj_arr as $key => $curl_obj) {
			$res[$key] = curl_multi_getcontent($curl_obj->getHandler());
			curl_multi_remove_handle($this->_multi_handler, $curl_obj->getHandler());
		}
		
		curl_multi_close($this->_multi_handler);
		\aquagear\log\Logger::getLogInstance()->info('curl multi close.');
	}
	
	/**
	 * デストラクタ
	 * 
	 * @return void
	 */
	public function __destruct() {
		$this->close();
	}
}
?>