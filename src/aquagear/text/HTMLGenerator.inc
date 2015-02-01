<?php
namespace aquagear\text;

/**
 * Htmlジェネレーター
 * 
 * @package aquagear
 * @subpackage text
 * 
 * @author motonari
 * @version $Id$
 * 
 * @TODO まだ、開発中。（動かないかも。）
 */
class HTMLGenerator extends \aquagear\Object {
	
	/**
	 * mapping
	 * 
	 * @var array
	 */
	private $conf_map = null;
	
	/**
	 * text mode.
	 * 
	 * @var int
	 */
	const TEXT = 0;

	/**
	 * calc mode.
	 * 
	 * @var int
	 */
	const CALC = 1;

	/**
	 * text struct.
	 * 
	 * @var int
	 */
	const TEXT_STRUCTURE = 0;

	/**
	 * calc struct.
	 * 
	 * @var int
	 */
	const CALC_STRUCTURE = 1;
	
	/**
	 * 計算開始文字
	 * 
	 * @var string
	 */
	const CALC_STRAT_SEPARATOR = '{';

	/**
	 * 計算終了文字
	 * 
	 * @var string
	 */
	const CALC_END_SEPARATOR   = '}';

	/**
	 * エスケープ文字
	 * 
	 * @var string
	 */
	const ESCAPE = '\\';
	
	/**
	 * スペース
	 * 
	 * @var string
	 */
	const SPACE  = ' ';
	
	/**
	 * シングルクオート
	 * 
	 * @var string
	 */
	const SQUOTE = '\'';
	
	/**
	 * ダブルクオート
	 * 
	 * @var string
	 */
	const DQUOTE = '"';
	
	/**
	 * 等号
	 * 
	 * @var string
	 */
	const EQUAL  = '=';
	
	/**
	 * コンストラクタ.
	 * 
	 * @return void
	 */
	public function __construct() {
		$this->conf_map = \aquagear\holder\ConfigureHolder::load(\aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') . '/html_generator_map.ini');
	}
	
	/**
	 * 実行メソッド.
	 * 
	 * @return unknown_type
	 */
	public function convertStrToHTML($str) {
		$structure = $this->strParse($str);
		return $this->constructObject($structure)->execute();
	}

	/**
	 * 文字列解析
	 * 
	 * @param string $str 解析する文字列
	 * 
	 * @return string 構文解析結果
	 */
	private function strParse($str) {
		$type = self::TEXT_STRUCTURE;
		$structure_arr = array();
		$struct_ch_arr = str_split(rtrim($str));
		$liner_str = '';
		for($i=0; ; $i++) {
			switch($struct_ch_arr[$i]) {
				case self::CALC_STRAT_SEPARATOR:
					$structure[] = array('str' => $liner_str, 'type' => $type);
					$type = self::CALC_STRUCTURE;
					$liner_str = '';
					break;
				case self::CALC_END_SEPARATOR:
					$structure[] = array('str' => $liner_str, 'type' => $type);
					$type = self::TEXT_STRUCTURE;
					$liner_str = '';
					break;
				case self::ESCAPE:
					if(isset($struct_ch_arr[++$i])) {
						$liner_str .= $struct_ch_arr[$i];
					}
					break;
				default:
					$liner_str .= $struct_ch_arr[$i];
					break;
			}
			
			if(!isset($struct_ch_arr[$i+1])) {
				$structure[] = array('str' => $liner_str, 'type' => self::TEXT_STRUCTURE);
				$liner_str = '';
				break;
			}
		}
		
		return $structure;
	}
	
	/**
	 * 文章解析結果から、オブジェクトを作成する。
	 * 
	 * @param array $struct_arr 文章解析結果
	 * 
	 * @return obj ルートオブジェクト
	 */
	private function constructObject(array $struct_arr) {
		$now_obj = new \aquagear\text\tags\HeadTag();
		$head = &$now_obj;
		$object_stack = array();
		foreach($struct_arr as $struct) {
			switch($struct['type']) {
				case self::TEXT:
					$obj = new \aquagear\text\tags\TextTag();
					$obj->init($struct['str']);
					$now_obj->pushObj($obj);
					break;
				case self::CALC:
					if(preg_match('/^\//', $struct['str']) === 0) {
						$obj = null;
						$calc_arr = $this->parseCalcString($struct['str']);
						if($this->conf_map->getValue($calc_arr['name']) !== false) {
							$class_name = $this->conf_map->getValue($calc_arr['name']);
							$obj = new $class_name;
							$obj->init($calc_arr['opt']);
						}
						else {
							$obj = new \aquagear\text\tags\HeadTag();
						}
						$object_stack[] = $now_obj;
						$now_obj = $obj;
					}
					else {
						$obj = array_pop($object_stack);
						$obj->pushObj($now_obj);
						$now_obj = $obj;
					}
					break;
			}
		}
		
		return $head;
	}
	
	/**
	 * 処理系の文字列のパース
	 * 
	 * @param string $str 文字列
	 * 
	 * @return array パース結果
	 * 
	 * @TODO 後で整理
	 */
	private function parseCalcString($str) {
		$type = 0;
		$opt_arr = explode(' ', $str,2);
		
		if(!isset($opt_arr[1])) {
			return array('name' => $opt_arr[0]);
		}
		
		$opt_list_arr = array();
		$str_flag = 0;
		$key = '';
		$liner = '';
		$opt_arr[1] = preg_replace('/[\s]*=[\s]*/', '=', $opt_arr[1]);
		$opt_str_ch_arr = str_split(trim($opt_arr[1]));
		for($i=0;;$i++) {
			switch($opt_str_ch_arr[$i]) {
				case self::DQUOTE:
					if($str_flag === 1) {
						$str_flag = 0;
					}
					else if($str_flag === 2) {
						$liner .= $opt_str_ch_arr[$i];
					}
					else {
						$str_flag = 1;
					}
					break;
				case self::SQUOTE:
					if($str_flag === 1) {
						$liner .= $opt_str_ch_arr[$i];
					}
					else if($str_flag === 2) {
						$str_flag = 0;
					}
					else {
						$str_flag = 2;
					}
					break;
				case self::EQUAL:
					if($str_flag === 0) {
						$type = 1;
						$key = $liner;
						$liner = '';
					}
					else {
						$liner .= $opt_str_ch_arr[$i];
					}
					break;
				case self::SPACE:
					if($str_flag === 0) {
						if($type === 0) {
							$opt_list_arr[$liner] = true;
						}
						else {
							$opt_list_arr[$key] = $liner;
						}
					
						$type = 0;
						$key = '';
						$liner = '';
					}
					else {
						$liner .= $opt_str_ch_arr[$i];
					}
					break;
				case self::ESCAPE:
					$i++;
					if(isset($opt_str_ch_arr[$i])) {
						$liner .= $opt_str_ch_arr[$i];
					}
					break;
				default:
					$liner .= $opt_str_ch_arr[$i];
					break;
			}

			if(!isset($opt_str_ch_arr[$i+1])) {
				if($type === 0) {
					$opt_list_arr[$liner] = true;
				}
				else {
					$opt_list_arr[$key] = $liner;
				}
				
				$type = 0;
				$key = '';
				$liner = '';
				
				break;
			}
		}
		
		return array('name' => $opt_arr[0],
						'opt' => $opt_list_arr);
	}
}
?>